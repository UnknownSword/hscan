.PHONY: all build safe run clean

CXX          := gcc
CXXFLAGS     := -Iinclude
SAFE_FLAGS   := -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -Wall -Wextra -g
LIBS :=

SRC_DIR      := src
INCLUDE_DIR  := include
BUILD_DIR    := build
BIN_DIR      := bin

MAIN_SRC     := main
#BINARY       := $(BIN_DIR)/$(MAIN_SRC)
BINARY       := ./hscan

SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -o -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) \
        $(BUILD_DIR)/$(MAIN_SRC).o

all: build run

build: $(BINARY)

$(BINARY): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $@ ..."
	@$(CXX) $(OBJS) $(CXXFLAGS) $(LIBS) -o $@
	@echo "Build successful! Run with: ./$@"

$(BUILD_DIR)/$(MAIN_SRC).o: $(MAIN_SRC).c
	@mkdir -p $(dir $@)
	@echo "Compiling $< ..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< ..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

safe: CXXFLAGS += $(SAFE_FLAGS)
safe: build

run:
	@if [ -f $(BINARY) ]; then \
		echo "Running $(BINARY) ..."; \
		$(BINARY); \
	else \
		echo "Error: Binary not found. First run 'make build'"; \
	fi

clean:
	@echo "Cleaning up ..."
	rm -rf $(BUILD_DIR) $(BIN_DIR) tmp $(BINARY)
