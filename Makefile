# === Directories ===
INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/objects
BIN_DIR := $(BUILD_DIR)/bin
LIB_RELEASE := $(BUILD_DIR)/lib/release
LIB_DEBUG := $(BUILD_DIR)/lib/debug

# === Compiler and Flags ===
CXX := g++
CXXFLAGS := -std=c++17 -Wall -I$(INCLUDE_DIR)

# === Source and Object Files ===
SRCS := $(wildcard $(SRC_DIR)/*.cpp)  # Automatically find all .cpp files in src/
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# === Binaries and Libraries ===
STATIC_LIB := $(LIB_RELEASE)/libhashmap.a
SHARED_LIB := $(LIB_RELEASE)/libhashmap.so
DEBUG_LIB := $(LIB_DEBUG)/libhashmap.so
TEST_EXE := $(BIN_DIR)/test_runner
BENCHMARK_SRC := tests/benchmarker.cpp
BENCH_EXE := $(BIN_DIR)/benchmarker

.PHONY: all static shared debug clean install test benchmark

# === Default Build ===
all: static shared

# === Static Library ===
static: $(STATIC_LIB)

$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_RELEASE)
	ar rcs $@ $^

# === Shared Library ===
shared: $(SHARED_LIB)

$(SHARED_LIB): $(OBJS)
	@mkdir -p $(LIB_RELEASE)
	$(CXX) -shared -o $@ $^

# === Debug Version ===
debug: CXXFLAGS += -g
debug: $(DEBUG_LIB)

$(DEBUG_LIB): $(OBJS)
	@mkdir -p $(LIB_DEBUG)
	$(CXX) -shared -o $@ $^

# === Object File Rule ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

# === Test Runner ===
test: $(TEST_EXE)
	./$(TEST_EXE)

$(TEST_EXE): tests/test.cpp $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Benchmark Runner ===
benchmark: $(BENCH_EXE)
	./$(BENCH_EXE)

$(BENCH_EXE): tests/benchmarker.cpp $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Install Shared Library ===
install: shared
	sudo cp $(SHARED_LIB) /usr/local/lib/

# === Clean ===
clean:
	rm -rf $(BUILD_DIR)

# $@ = target name (e.g., libhashmap.a or test_runner)
# $^ = all prerequisites (e.g., lib.o or main.o)
