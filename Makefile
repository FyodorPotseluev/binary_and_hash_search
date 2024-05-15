
#   $@ - current target name;
#   $< - name of the first target in the list of dependencies;
#   $^ - full list of dependencies;
#   @  - symbol before a command suppresses the printing of the command itself.
#        It's a way to make the command execute quietly, without displaying the
#        command itself in the terminal output.
#   %  - searches for a pattern in OBJMODULES variable and replace all `%` in
#   	 the current target with this pattern;
#   MAKECMDGOALS
#      - if you run `make target1 target2` MAKECMDGOALS variable will contain
#      	 all the targets: `target1` and `target2`;
#   deps.mk
#      - contains all dependencies for all the project's `*.c` files.
#        We paste the contents of `deps.mk` file in the text with `-include`
#        directive. It consists of the lines like the one below:
#
#      mod1.o: mod1.c mod1.h mod2.h mod3.h
#
#        mod1.o file is build according to general rule (there is no any
#        special commands for this target). But it's rebuilt every time
#        any file from its dependency list was changed.
#
#      deps.mk: $(SRCMODULES)
#              $(CC) -MM $^ > $@
#
#        automatically creates dependencies list so we don't need to track
#        them manually;
#   :=
#      - is often preferred when you want to ensure that the value of a variable
#   	 is fixed and constant throughout the execution;
#   =
#      - is useful when you want to allow for the possibility of the variable's
#   	 value changing during the Makefile's execution;
#   | $(bin_dir)
#      - "order-only prerequisite". If a directory prerequisite changes (for
#        example new files were added to the directory), it won't trigger a
#        rebuild of the target as long as the actual dependencies haven't
#        changed;
#   $(patsubst pattern,replacement,text)
#      - function which finds whitespace-separated words in `text` that match
#        `pattern` and replaces them with `replacement`;
#   $(firstword $(list_of_words))
#      - this function returns the first word in a given list of words;
#   $(words $(list_of_words))
#      - this function returns the number of words a given list of words;
#   VAR := $(wordlist from,to,$(list_of_words))
#      - the `wordlist` function extracts a sublist of words from a given list.
#        The first argument `from` specifies the starting index of the sublist.
#        The second argument `to` specifies the ending index of the sublist;
#   $(eval $(ARGS):;@:)
#      - `eval` function dynamically generates targets based on `ARGS` contents;
#      - `:` is a syntax used in Makefiles to define targets and prerequisites;
#      - `;@:` is a do-nothing recipe for each generated target. This ensures
#        that the dynamically generated targets don't actually do anything when
#        they're built. The semicolon `;` separates multiple commands, and `@:`
#        is a silent command that does nothing.
#        Overall, using `eval` function is a measure to ensure a smoother
#        execution and to suppress any warning messages about missing rules for
#        the specified targets.
#        Another `eval` function usage in the program
#
#      $(eval ARGS := $(...))
#
#      	 helps to overcom the issue that a variable declared inside a user
#      	 function expands immediately, even if the function is not yet called.
#	 `eval` usage defers the expansion of ARGS until the user function is
#	 used.

PROJECT := database_binary_and_hash_search

# Variables for paths of source, header, and test files
INC_DIR := ./include
SRC_DIR := ./src
SRCMODULES := $(wildcard $(SRC_DIR)/*.c)
TEST_DIR := ./test

# Variables for paths of object files and binary targets
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
EXECUTABLE := $(BIN_DIR)/$(PROJECT)
BUILD_DIRS := $(OBJ_DIR) $(BIN_DIR)
OBJMODULES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCMODULES))

# C compiler configuration
CC = gcc # using gcc compiler
CFLAGS = -Wall -Wextra -g -O0 -Iinclude
# CFLAGS options:
# -Wall		Warnings: all - display every single warning
# -Wextra	Enable some extra warning flags that are not enabled by -Wall
# -g		Compile with debug symbols in binary files
# -O0		Disable compilation optimizations
# -Iinclude	Add the directory /include to the list of directories to be
#  		searched for header files during preprocessing.

# Function to parse command line arguments.
# Unfortunatelly using the function causes the following error:
# Makefile:109: *** recipe commences before first target.  Stop.
# and I currently don't know how to fix it.
# define parse_args
	# Extract the rest of the arguments after the first one
	# $(eval ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS)))
	# ...and turn them into do-nothing targets
	# $(eval $(ARGS):;@:)
# endef

# If the first argument is "run" or "debug"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
	# use the rest as arguments for "run"
	ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  	# ...and turn them into do-nothing targets
  	$(eval $(ARGS):dummy;@:)
	# the correct way to avoid code duplication
	# $(call parse_args)
else ifeq (debug,$(firstword $(MAKECMDGOALS)))
	# use the rest as arguments for "run"
	ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  	# ...and turn them into do-nothing targets
  	$(eval $(ARGS):dummy;@:)
	# the correct way to avoid code duplication
	# $(call parse_args)
else ifeq (leak_search,$(firstword $(MAKECMDGOALS)))
	# use the rest as arguments for "run"
	ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  	# ...and turn them into do-nothing targets
  	$(eval $(ARGS):dummy;@:)
	# the correct way to avoid code duplication
	# $(call parse_args)
endif

all: $(EXECUTABLE)

# Display useful goals in this Makefile
help:
	@echo "Try one of the following make goals:"
	@echo " > all - build project"
	@echo " > readme - project's documentation"
	@echo " > run arg1 arg2 arg3* - execute the project"
	@echo " > input_output_test - run the project's integration test"
	@echo " > memcheck_test - run the project's memory check test"
	@echo " > debug arg1 arg2 arg3* - begin a gdb process for the executable"
	@echo " > leak_search arg1 arg2 arg3* - run the project under valgrind"
	@echo " > clean - delete build files in project"
	@echo " > variables - print Makefile's variables"
	@echo " * the arguments cannot:"
	@echo "    - match existing Makefile targets;"
	@echo "    - contain quotation marks, space characters;"
	@echo "    - begin with a hyphen-minus symbol."
	@echo " To avoid restrictions please run the project as"
	@echo " build/bin/database_binary_and_hash_search arg1 arg2 arg3"

# Build the project by combining all object files
$(EXECUTABLE): $(OBJMODULES) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Build object files from sources in a template pattern
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

deps.mk: $(SRCMODULES)
	$(CC) -MM -Iinclude $^ > $@

readme:
ifdef EDITOR
	$(EDITOR) README.txt
else
	xdg-open README.txt
endif

run: $(EXECUTABLE)
	@$(EXECUTABLE) $(ARGS)

input_output_test:
	$(TEST_DIR)/input_output_test.sh

memcheck_test:
	$(TEST_DIR)/memory_leak_test.sh

debug:
	gdb --args $(EXECUTABLE) $(ARGS)

leak_search:
	valgrind --tool=memcheck --leak-check=full --errors-for-leak-kinds=definite,indirect,possible --show-leak-kinds=definite,indirect,possible $(EXECUTABLE) $(ARGS)

# set 1st cmd line argument as phony target. Prevents frome `make` warnings like
# ```make: 'file' is up to date.```
.PHONY: $(firstword $(ARGS))

clean:
	rm -f $(OBJ_DIR)/* $(EXECUTABLE)

variables:
	@echo "PROJECT =" $(PROJECT)
	@echo
	@echo "# Variables for paths of source, header, and test files"
	@echo "INC_DIR =" $(INC_DIR)
	@echo "SRC_DIR =" $(SRC_DIR)
	@echo "SRCMODULES =" $(SRCMODULES)
	@echo "TEST_DIR =" $(TEST_DIR)
	@echo
	@echo "# Variables for paths of object files and binary targets"
	@echo "BUILD_DIR =" $(BUILD_DIR)
	@echo "OBJ_DIR =" $(OBJ_DIR)
	@echo "BIN_DIR =" $(BIN_DIR)
	@echo "EXECUTABLE =" $(EXECUTABLE)
	@echo "BUILD_DIRS =" $(BUILD_DIRS)
	@echo "OBJMODULES =" $(OBJMODULES)
	@echo
	@echo "# C compiler configuration"
	@echo "CC =" $(CC)
	@echo "CFLAGS =" $(CFLAGS)
