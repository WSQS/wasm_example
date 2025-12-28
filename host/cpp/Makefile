# Makefile for C++ Host with Wasmtime C API

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = calculator-host
SRC = main.cpp

# Wasmtime paths
# Default to project-local wasmtime-dist, but can be overridden via WASMTIME_DIST
WASMTIME_DIST ?= $(CURDIR)/wasmtime-dist
WASMTIME_INCLUDE = $(WASMTIME_DIST)/include
WASMTIME_LIB = $(WASMTIME_DIST)/lib

# Default target
all: $(TARGET)

# Build the target with Wasmtime C API
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(WASMTIME_INCLUDE) -o $(TARGET) $(SRC) \
		-L$(WASMTIME_LIB) -lwasmtime -ldl -lpthread -lm

# Run with default arguments
run: $(TARGET)
	LD_LIBRARY_PATH=$(WASMTIME_LIB) ./$(TARGET)

# Run with custom arguments
run-custom: $(TARGET)
	LD_LIBRARY_PATH=$(WASMTIME_LIB) ./$(TARGET) --wasm ../../calculator/composed.wasm --expr "123"

# Clean build artifacts
clean:
	rm -f $(TARGET)

# Install target (optional)
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

.PHONY: all run run-custom clean install