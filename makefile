# Deal with the difference of the operating system
ifdef OS
	FixExecutable = $1.exe
	Remove        = $(shell for %%f in ($1) do if exist %%f del /q %%f)
	FixPath       = $(subst /,\, $1)
	Mkdir         = $(shell for %%f in ($1) do if not exist %%f mkdir %%f)
else
	ifeq ($(shell uname), Linux)
		FixExecutable = $1
		Remove        = rm -f $1
		FixPath       = $1
		Mkdir         = mkdir -p $1
	endif
endif
# Message function
MakeMsg = $(info Make Message:$1)
# Comma
comma :=,

# Define a recursive wildcard function
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))


####################################################
# Compile
####################################################
# Target file
TARGET := Test
TARGET := $(call FixExecutable,$(TARGET))

# CPP compiler
CXX := g++

# CPP version
CXXVERSION := c++20

# Directories
TARGET_DIR  := build/bin
SRC_DIR     := src
INCLUDE_DIR := include
LIB_DIR     := lib
OBJ_DIR     := objs

# Compiler flags
CXXFLAGS := -g -Wall -std=$(CXXVERSION) -I$(INCLUDE_DIR) -Wextra -MMD -MP

# Linker flags
LDFLAGS := -L$(LIB_DIR)

# Source files
CPP_SRCS := $(call rwildcard,$(SRC_DIR)/,*.cpp)
C_SRCS   := $(call rwildcard,$(SRC_DIR)/,*.c)
SRCS     := $(CPP_SRCS) $(C_SRCS)

# Object files
OBJS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(CPP_SRCS))
OBJS += $(patsubst %.c, $(OBJ_DIR)/%.o, $(C_SRCS))

# Dependency files
DEPS := $(OBJS:.o=.d)

# Directories needed by object files and dependency files
MIRROR_DIR := $(dir $(OBJS))
MIRROR_DIR := $(MIRROR_DIR:/=)
MIRROR_DIR := $(sort $(MIRROR_DIR))

# Append directory to target file
BUILDTARGET := $(TARGET_DIR)/$(TARGET)

# Default target
all: $(BUILDTARGET)

# Rule to link executable
$(BUILDTARGET): $(OBJS) | $(TARGET_DIR)
	$(call MakeMsg, Starting linking...)
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	$(call MakeMsg, Building finished!)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: %.cpp | $(MIRROR_DIR)
	$(call MakeMsg, Compiling C++ source file to object file: $<)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	$(call MakeMsg, Compiling finished!)

# Rule to compile C source files into object files
$(OBJ_DIR)/%.o: %.c | $(MIRROR_DIR)
	$(call MakeMsg, Compiling C source file to object file: $<)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	$(call MakeMsg, Compiling finished!)

# Ensure the build directory exists
$(TARGET_DIR):
	$(call MakeMsg, Deteced missing directory "$@"$(comma) create new one...)
	@$(call Mkdir, $(call FixPath, $(TARGET_DIR)))
	$(call MakeMsg, Done!)

# Ensure the build directory exists
$(MIRROR_DIR):
	$(call MakeMsg, Deteced missing directory "$@"$(comma) create new one...)
	@$(call Mkdir, $(call FixPath, $(MIRROR_DIR)))
	$(call MakeMsg, Done!)

# Include dependency files if they exist
-include $(DEPS)


####################################################
# -- init --
# This call help you to create the directories
# stucture which fit this makefile.
####################################################
# Directories to be create
MKDIRS := 	$(SRC_DIR)	\
		$(INCLUDE_DIR)	\
		$(LIB_DIR)

# Run method
init:
	$(call MakeMsg, Starting to initialize workspace...)
	@$(call Mkdir, $(call FixPath, $(MKDIRS)))
	$(call MakeMsg, Initialization completed!)
	@cd .


####################################################
# -- run --
# This call help you to compile and run project.
####################################################
# File to be executed
EXECUTE			:= ./$(TARGET)
EXECUTEFLAGS	:=

# Run method
run: $(BUILDTARGET)
	$(call MakeMsg, Start running program...)
	$(info -------------------------- Start Running --------------------------)
	@cd $(TARGET_DIR) && $(call FixPath, $(EXECUTE) $(EXECUTEFLAGS))


####################################################
# -- clean --
# This call help you to clean up files generated
# by compiler.
####################################################
# Files to be cleaned
# (Because of skill issue, please specify files instead of directories.)
RM_TARGETS := 	$(BUILDTARGET)		\
				$(OBJS)				\
				$(DEPS)				\
				$(TARGET_DIR)/*.ppm	\
				$(TARGET_DIR)/*.png	\
				$(TARGET_DIR)/*.jpg

# Clean up build directory
clean:
	$(call MakeMsg, Starting to delete created files...)
	@$(call Remove, $(call FixPath, $(RM_TARGETS)))
	$(call MakeMsg, Done!)
	@cd .


# Phony targets
.PHONY: all init run clean