####################################################
# Compile
####################################################
# Target file
TARGET := Test

# CPP compiler
CXX := g++

# CPP version
CXXVERSION := c++20

# Directories
TARGET_DIR  := build/bin
SRC_DIR     := src
BUILD_DIR   := objs
INCLUDE_DIR := include

# Compiler flags
CXXFLAGS := -g -Wall -std=$(CXXVERSION) -I$(INCLUDE_DIR) -Wextra -MMD -MP

# Linker flags
LDFLAGS :=

# Source files
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS   := $(wildcard $(SRC_DIR)/*.c)
SRCS     := $(CPP_SRCS) $(C_SRCS)

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_SRCS))
OBJS += $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))

# Dependency files
DEPS := $(OBJS:.o=.d)


# Deal with the difference of the operating system
ifdef OS
	TARGET       := $(TARGET).exe
	RM            = del /Q
	FixPath       = $(subst /,\, $(1))
	MKDIR         = mkdir
else
	ifeq ($(shell uname), Linux)
		RM        = rm -f
		FixPath   = $(1)
		MKDIR     = mkdir -p
	endif
endif

# Append directory to target file
BUILDTARGET := $(TARGET_DIR)/$(TARGET)

# Default target
all: $(BUILDTARGET)

# Rule to link executable
$(BUILDTARGET): $(OBJS) | $(TARGET_DIR)
	@echo Starting linking...
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo Building finished!

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo Compiling C++ source file to object file: $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo Compiling C source file to object file: $<
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure the build directory exists
$(TARGET_DIR):
	@$(MKDIR) $(call FixPath, $(TARGET_DIR))

# Ensure the build directory exists
$(BUILD_DIR):
	@$(MKDIR) $(call FixPath, $(BUILD_DIR))

# Include dependency files if they exist
-include $(DEPS)


####################################################
# Clean
####################################################
# Files to be cleaned
RM_FILES := $(BUILDTARGET)       \
			$(OBJS)              \
			$(DEPS)              \
			$(TARGET_DIR)/*.ppm  \
			$(TARGET_DIR)/*.png  \
			$(TARGET_DIR)/*.jpg

# Clean up build directory
clean:
	@echo Starting to delete created files...
	@$(RM) $(call FixPath, $(RM_FILES))
	@echo Done!

####################################################
# Run
####################################################
# File to be executed
EXECUTE := ./$(TARGET)

# Run method
run: $(BUILDTARGET)
	@echo -------------------------- Start Running --------------------------
	@cd $(TARGET_DIR) && $(call FixPath, $(EXECUTE))
	@echo -------------------------- End Running --------------------------

# Phony targets
.PHONY: all clean run
