# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -Iinclude

# Directories
SRC_DIR := src
BUILD_DIR := build
TEST_DIR := tests

# Target (final executable in project root)
TARGET := main

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Find all test .cpp files and their executables
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TESTS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%,$(TEST_SRCS))

# Default rule
all: $(TARGET)

# Link object files into final executable (in project root)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile each .cpp to .o inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule for building tests
# Each test is compiled from tests/*.cpp + all src objects
$(BUILD_DIR)/%: $(TEST_DIR)/%.cpp $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build and run all tests
.PHONY: test
test: $(TESTS)
	@for t in $^; do ./$$t; done

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
