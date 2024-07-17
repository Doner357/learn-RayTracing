# Setup project name
PROJECT_NAME := Test

# Deal the difference of the operating system
ifdef OS
	RM      = del /Q
	FixPath = $(subst /,\, $(1))
	MKDIR   = mkdir
else
	ifeq ($(shell uname), Linux)
		RM      = rm -f
		FixPath = $(1)
		MKDIR   = mkdir -p
   endif
endif

# CPP compiler
CXX := g++

# CPP version
CXXVERSION := c++20

# Directories
TARGET_DIR  := bin
SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := include

# Compiler flags
CXXFLAGS := -g -Wall -std=$(CXXVERSION) -I$(INCLUDE_DIR) -Wextra -MMD -MP

# Linker flags
LDFLAGS :=

# Target file
TARGET := $(TARGET_DIR)/$(PROJECT_NAME)

# Source files
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)
SRCS     := $(CPP_SRCS) $(C_SRCS)

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_SRCS))
OBJS += $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))

# Dependency files
DEPS := $(OBJS:.o=.d)

# Default target
all: $(TARGET)
	@echo Building finished!

# Rule to link executable
$(TARGET): $(OBJS) | $(TARGET_DIR)
	@echo Start linking...
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo Start compiling cpp souce files to object files...
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo Start compiling c souce files to object files...
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure the build directory exists
$(TARGET_DIR):
	@$(MKDIR) $(TARGET_DIR)

# Ensure the build directory exists
$(BUILD_DIR):
	@$(MKDIR) $(BUILD_DIR)

# Include dependency files if they exist
-include $(DEPS)

# Phony targets
.PHONY: all clean

# Clean up build directory
clean:
	@echo Start deleting created files...
	@$(RM) $(call FixPath, $(OBJS))
	@$(RM) $(call FixPath, $(DEPS))
	@echo Done!