# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin
TESTS_DIR = tests

# Source files and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Target executable
TARGET = $(BIN_DIR)/HashMapApp

# Default rule: build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking object files to create executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $< into $@..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	@echo "Creating build directory..."
	mkdir -p $(BUILD_DIR)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	@echo "Creating bin directory..."
	mkdir -p $(BIN_DIR)

# Clean up build artifacts
clean:
	@echo "Cleaning up build artifacts..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the program
run: all
	@echo "Running the program..."
	./$(TARGET)

# Test rule (optional, if you want to run tests)
test: all
	@echo "Running tests from $(TESTS_DIR)/test.txt..."
	./$(TARGET) < $(TESTS_DIR)/test.txt

.PHONY: all clean run test
