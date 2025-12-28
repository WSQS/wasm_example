calculator.wasm: bindings
	/opt/wasi-sdk/bin/wasm32-wasip2-clang++ -o calculator.wasm -mexec-model=reactor calculator_cpp.cpp calculator.cpp calculator_component_type.o -std=c++20
bindings: wit/world.wit
	wit-bindgen cpp wit

