# C++ Host for WebAssembly Component Model

This is a C++ host program that can load and execute WebAssembly Component Model components using the Wasmtime C++ API with direct embedding.

## Features

- Load WebAssembly Component Model components using Wasmtime C++ API
- Call exported functions from components
- Command-line interface for specifying component path and function arguments
- Error handling and validation
- Direct embedding (no subprocess spawning)

## Requirements

- C++17 compatible compiler (g++, clang++)
- Wasmtime 40.0.0 or later with C++ API support
- Make

## Wasmtime Dependencies

The host uses Wasmtime C++ API for embedding WebAssembly components. The project includes a pre-built distribution of Wasmtime in the `wasmtime-dist/` directory.

### Default Setup

By default, the build system uses the project-local Wasmtime distribution:
- **Headers**: `wasmtime-dist/include/`
- **Libraries**: `wasmtime-dist/lib/`

This distribution is included in the repository and should work out of the box.

### Custom Wasmtime Path

If you need to use a different Wasmtime installation, you can override the default path using the `WASMTIME_DIST` environment variable:

```bash
# Build with custom Wasmtime distribution
WASMTIME_DIST=/path/to/wasmtime-dist make

# Run with custom Wasmtime distribution
WASMTIME_DIST=/path/to/wasmtime-dist make run
```

### Preparing Wasmtime Distribution

If the included `wasmtime-dist/` directory is missing or you want to update it, you can prepare a new distribution:

#### Option 1: Copy from existing Wasmtime build

If you have Wasmtime built elsewhere (e.g., in `/tmp/wasmtime/`):

```bash
# Create directory structure
mkdir -p wasmtime-dist/include wasmtime-dist/lib

# Copy headers
cp -r /path/to/wasmtime/crates/c-api/include/* wasmtime-dist/include/

# Copy libraries (adjust architecture as needed)
cp /path/to/wasmtime/target/x86_64-unknown-linux-gnu/release/libwasmtime.* wasmtime-dist/lib/
```

#### Option 2: Build Wasmtime from source

```bash
# Clone Wasmtime repository
git clone https://github.com/bytecodealliance/wasmtime.git
cd wasmtime

# Build release version with C API
cargo build --release

# Create distribution directory
mkdir -p ../host/cpp/wasmtime-dist/include ../host/cpp/wasmtime-dist/lib

# Copy headers
cp -r crates/c-api/include/* ../host/cpp/wasmtime-dist/include/

# Copy libraries
cp target/x86_64-unknown-linux-gnu/release/libwasmtime.* ../host/cpp/wasmtime-dist/lib/
```

#### Option 3: Download pre-built distribution

If pre-built distributions are available, download and extract them to `wasmtime-dist/`.

### Required Files

The `wasmtime-dist/` directory must contain:

**In `include/`:**
- `wasm.h`
- `wasmtime.hh`
- `wasmtime/component.hh`
- `wasmtime/store.hh`
- All other Wasmtime C++ API headers

**In `lib/`:**
- `libwasmtime.so` (shared library)
- `libwasmtime.a` (static library, optional)

## Building

```bash
make
```

This will create the `calculator-host` executable.

## Running

### Default usage (uses ../../calculator/composed.wasm and expression "123")

```bash
./calculator-host
```

### Custom component path

```bash
./calculator-host --wasm path/to/component.wasm
```

### Custom expression

```bash
./calculator-host --expr "456"
```

### Both custom component and expression

```bash
./calculator-host --wasm ../../calculator/composed.wasm --expr "1+2"
```

### Using make

```bash
make run-custom
```

## Example Output

```bash
$ ./calculator-host --wasm ../../calculator/composed.wasm --expr "123"
Result: 3
```

## Command-line Options

- `--wasm <path>`: Path to the WebAssembly component file (default: ../../calculator/composed.wasm)
- `--expr <string>`: Expression to evaluate (default: "123")
- `--help, -h`: Show help message

## Implementation Details

This C++ host uses the Wasmtime C++ API with direct embedding to execute WebAssembly Component Model components. The implementation:

- Uses `wasmtime::Engine` to create the compilation environment
- Uses `wasmtime::component::Component` to compile component binaries
- Uses `wasmtime::Store` to manage component instances
- Uses `wasmtime::component::Linker` to link imports and instantiate components
- Uses `wasmtime::component::Instance` to access component exports
- Uses `wasmtime::component::Func` to call exported functions

### Component Export Lookup

The host demonstrates how to lookup nested exports in components:
1. First, lookup the top-level export (e.g., `docs:calculator/calculate@0.1.0`)
2. Then, lookup the nested function within that export (e.g., `eval-expression`)
3. Finally, get the function and call it with the appropriate arguments

### Error Handling

The host includes comprehensive error handling for:
- File not found errors
- Component compilation errors
- Linking and instantiation errors
- Function call errors
- Type validation errors

## Dependencies

The host uses the project-local Wasmtime distribution in `wasmtime-dist/` by default. This distribution includes:

- Wasmtime C++ API headers (version 40.0.0)
- Wasmtime shared library (libwasmtime.so)

No external installation of Wasmtime is required for building the host.

## License

Same as the parent project.