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
SRCS := $(SRC_DIR)/lib.cpp
OBJS := $(OBJ_DIR)/lib.o

# === Binaries and Libraries ===
STATIC_LIB := $(LIB_RELEASE)/libhashset.a
SHARED_LIB := $(LIB_RELEASE)/libhashset.so
DEBUG_LIB := $(LIB_DEBUG)/libhashset.so
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

$(TEST_EXE): $(SRC_DIR)/main.cpp $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Benchmark Runner ===
benchmark: $(BENCH_EXE)
	./$(BENCH_EXE)

$(BENCH_EXE): $(BENCHMARK_SRC) $(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Install Shared Library ===
install: $(SHARED_LIB)
	sudo cp $(SHARED_LIB) /usr/local/lib/

# === Clean ===
clean:
	rm -rf $(BUILD_DIR)

# $@ = target name (libhashset.a)
# $^ = all prerequisites (lib.o)
