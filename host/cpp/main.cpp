#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <wasmtime.hh>
#include <wasmtime/component.hh>
#include <wasmtime/store.hh>

namespace fs = std::filesystem;

using namespace wasmtime;
using namespace wasmtime::component;

// Read file contents into a vector
std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + path);
    }

    return buffer;
}

int main(int argc, char* argv[]) {
    // Default values
    std::string wasm_path = "../../calculator/composed.wasm";
    std::string expr = "123";

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--wasm" && i + 1 < argc) {
            wasm_path = argv[++i];
        } else if (arg == "--expr" && i + 1 < argc) {
            expr = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [OPTIONS]\n"
                      << "Options:\n"
                      << "  --wasm <path>    Path to the WebAssembly component file\n"
                      << "                    (default: ../../calculator/composed.wasm)\n"
                      << "  --expr <string>  Expression to evaluate\n"
                      << "                    (default: \"123\")\n"
                      << "  --help, -h       Show this help message\n";
            return 0;
        }
    }

    // Check if wasm file exists
    if (!fs::exists(wasm_path)) {
        std::cerr << "Error: WASM file not found: " << wasm_path << std::endl;
        return 1;
    }

    try {
        // Create engine
        Engine engine;

        // Read wasm file
        auto wasm_bytes = read_file(wasm_path);

        // Compile component
        auto component = Component::compile(engine, wasm_bytes).unwrap();

        // Create store
        Store store(engine);
        auto context = store.context();

        // Create linker
        wasmtime::component::Linker linker(engine);

        // Define unknown imports as traps
        linker.define_unknown_imports_as_traps(component).unwrap();

        // Instantiate component
        auto instance = linker.instantiate(context, component).unwrap();

        // Get export index for "docs:calculator/calculate@0.1.0"
        auto calculate_index = component.export_index(nullptr, "docs:calculator/calculate@0.1.0");
        if (!calculate_index) {
            std::cerr << "Error: Export 'docs:calculator/calculate@0.1.0' not found in component" << std::endl;
            return 1;
        }

        // Get export index for "eval-expression" within calculate interface
        auto export_index = component.export_index(&calculate_index.value(), "eval-expression");
        if (!export_index) {
            std::cerr << "Error: Export 'eval-expression' not found in component" << std::endl;
            return 1;
        }

        // Get function
        auto func = instance.get_func(context, export_index.value());
        if (!func) {
            std::cerr << "Error: Failed to get function 'eval-expression'" << std::endl;
            return 1;
        }

        // Prepare arguments (string parameter)
        std::array<wasmtime::component::Val, 1> args{
            wasmtime::component::Val::string(expr)
        };

        // Prepare results (u32 return value)
        std::array<wasmtime::component::Val, 1> results{
            wasmtime::component::Val(uint32_t(0))
        };

        // Call function
        func->call(context, args, results).unwrap();

        // Post-return cleanup
        func->post_return(context).unwrap();

        // Get result
        if (!results[0].is_u32()) {
            std::cerr << "Error: Unexpected result type" << std::endl;
            return 1;
        }

        uint32_t result = results[0].get_u32();
        std::cout << "Result: " << result << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}