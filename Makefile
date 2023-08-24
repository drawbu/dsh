CC := gcc -std=gnu11
CFLAGS := -W -Wall -Wextra
CFLAGS += -U_FORTIFY_SOURCE
CFLAGS += -iquote ./include

NAME := dsh
NAME_DEBUG := debug
NAME_TESTS := test
NAME_AFL := afl_dsh

ifneq ($(shell find . -name ".fast"),)
	MAKEFLAGS += -j
endif

BUILD_DIR := .build

# ↓ Sources
VPATH := src
VPATH += src/args
VPATH += src/shell
SRC := main.c
SRC += shell.c
SRC += shell_input.c
SRC += args.c
SRC += args_parser.c
SRC += env.c

SRC_DEBUG := $(SRC)

VPATH += tests
SRC_TESTS := $(SRC)
SRC_TESTS += test_sentinel.c
SRC_TESTS += test_process_input.c
SRC_TESTS += test_args.c
SRC_TESTS += test_args_parser.c

# ↓ Objects
OBJ := $(SRC:%.c=$(BUILD_DIR)/release/%.o)
OBJ_DEBUG := $(SRC_DEBUG:%.c=$(BUILD_DIR)/debug/%.o)
OBJ_TESTS := $(SRC_TESTS:%.c=$(BUILD_DIR)/tests/%.o)
OBJ_AFL := $(SRC:%.c=$(BUILD_DIR)/afl/%.o)

# ↓ Dependencies for headers
DEPS_FLAGS := -MMD -MP
DEPS := $(OBJ:.o=.d)
DEPS_DEBUG := $(OBJ_DEBUG:.o=.d)
DEPS_TESTS := $(OBJ_TESTS:.o=.d)

# ↓ Colors
ECHO := echo -e
ifneq ($(shell tput colors),0)
    C_RESET := \033[00m
    C_BOLD := \e[1m
    C_RED := \e[31m
    C_GREEN := \e[32m
    C_YELLOW := \e[33m
    C_BLUE := \e[34m
    C_PURPLE := \e[35m
    C_CYAN := \e[36m
endif

DIE := exit 1

all: $(NAME)

.PHONY: all

# ↓ Compiling release
$(BUILD_DIR)/release/%.o: %.c
	@ mkdir -p $(dir $@)
	@ $(ECHO) "[$(C_RED)$(C_BOLD)CC$(C_RESET)] $(C_GREEN)$^$(C_RESET)"
	@ $(CC) -o $@ -c $< $(CFLAGS) $(DEPS_FLAGS) || $(DIE)

$(NAME): $(OBJ)
	@ $(ECHO) "[$(C_RED)$(C_BOLD)CC$(C_RESET)] $(C_PURPLE)$@$(C_RESET)"
	@ $(CC) -o $@ $^ $(CFLAGS) || $(DIE)

# ↓ Compiling debug
$(BUILD_DIR)/debug/%.o: %.c
	@ mkdir -p $(dir $@)
	@ $(ECHO) "[$(C_RED)$(C_BOLD)CC$(C_RESET)] $(C_GREEN)$^$(C_RESET)"
	@ $(CC) -o $@ -c $< $(CFLAGS) $(DEPS_FLAGS) || $(DIE)

$(NAME_DEBUG): CFLAGS += -fsanitize=address,leak,undefined -g3
$(NAME_DEBUG): CFLAGS += -D DEBUG_MODE
$(NAME_DEBUG): $(OBJ_DEBUG)
	@ $(ECHO) "[$(C_RED)$(C_BOLD)CC$(C_RESET)] $(C_PURPLE)$@$(C_RESET)"
	@ $(CC) -o $@ $^ $(CFLAGS) || $(DIE)

# ↓ Compiling tests
$(BUILD_DIR)/tests/%.o: %.c
	@ mkdir -p $(dir $@)
	@ $(ECHO) "[$(C_RED)$(C_BOLD)CC$(C_RESET)] $(C_GREEN)$^$(C_RESET)"
	@ $(CC) -o $@ -c $< $(CFLAGS) $(DEPS_FLAGS) || $(DIE)

ifneq ($(NO_COV),1)
$(NAME_TESTS): CFLAGS += -g3 --coverage
$(NAME_TESTS): CFLAGS += -fprofile-arcs -ftest-coverage
endif
$(NAME_TESTS): CFLAGS += -D TEST_MODE
$(NAME_TESTS): CFLAGS += -lcriterion
$(NAME_TESTS): $(OBJ_TESTS)
	@ $(ECHO) "[$(C_RED)$(C_BOLD)CC$(C_RESET)] $(C_PURPLE)$@$(C_RESET)"
	@ $(CC) -o $@ $^ $(CFLAGS) || $(DIE)

run_tests: $(NAME_TESTS)
	@ ./$(NAME_TESTS) --verbose

.PHONY: run_tests

run_commands: $(NAME_DEBUG)
	@ cat tests/inputs/commands/dict.txt | ./$(NAME_DEBUG)

.PHONY: run_commands

# ↓ Compiling afl
$(BUILD_DIR)/afl/%.o: %.c
	@ mkdir -p $(dir $@)
	@ $(ECHO) "[$(C_RED)$(C_BOLD)CC$(C_RESET)] $(C_GREEN)$^$(C_RESET)"
	@ $(CC) -o $@ -c $< $(CFLAGS) $(DEPS_FLAGS) || $(DIE)

$(NAME_AFL): CC := afl-gcc
$(NAME_AFL): CFLAGS += -g3 -march=native -fsanitize=address
$(NAME_AFL): CFLAGS += -iquote tests/include
$(NAME_AFL): $(OBJ_AFL)
	@ AFL_USE_ASAN=1 $(CC) $(CFLAGS) -o $@ $^
	@ $(ECHO) "[$(C_RED)$(C_BOLD)AFL$(C_RESET)] $(C_GREEN)$^$(C_RESET)"

afl_run: $(NAME_AFL)
	@ $(RM) -r tests/generated
	@ afl-fuzz \
		-o tests/generated           \
		-m none                      \
		-i tests/inputs/commands     \
		-x tests/inputs/tokens       \
		-- ./$(NAME_AFL)

# ↓ Clean rules
clean:
	$(RM) -r $(BUILD_DIR)

fclean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r tests/generated
	$(RM) $(NAME) $(NAME_DEBUG) $(NAME_TESTS) $(NAME_AFL)

.PHONY: clean fclean

re: fclean
	$(MAKE) -s all

.PHONY: re

-include $(DEPS)
-include $(DEPS_DEBUG)
-include $(DEPS_TESTS)
