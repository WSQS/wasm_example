adder.wasm: bindings
	/opt/wasi-sdk/bin/wasm32-wasip2-clang -o adder.wasm -mexec-model=reactor component.cpp adder.cpp adder_component_type.o

bindings: world.wit
	wit-bindgen cpp world.wit

