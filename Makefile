CC := gcc -std=gnu11
CFLAGS := -W -Wall -Wextra
CFLAGS += -U_FORTIFY_SOURCE
CFLAGS += -iquote ./include

NAME := dsh
NAME_DEBUG := debug
NAME_TESTS := test

ifneq ($(shell find . -name ".fast"),)
	MAKEFLAGS += -j
endif

BUILD_DIR := .build

# ↓ Sources
VPATH := src
SRC := main.c
SRC += repl.c
SRC += args.c

SRC_DEBUG := $(SRC)

VPATH += tests
SRC_TESTS := $(SRC)
SRC_TESTS += test_sentinel.c
SRC_TESTS += test_process_input.c
SRC_TESTS += test_args.c

# ↓ Objects
OBJ := $(SRC:%.c=$(BUILD_DIR)/release/%.o)
OBJ_DEBUG := $(SRC_DEBUG:%.c=$(BUILD_DIR)/debug/%.o)
OBJ_TESTS := $(SRC_TESTS:%.c=$(BUILD_DIR)/tests/%.o)

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

$(NAME_DEBUG): CFLAGS += -fsanitize=address,leak,undefined
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

# ↓ Clean rules
clean:
	$(RM) -r $(BUILD_DIR)

fclean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(NAME) $(NAME_DEBUG) $(NAME_TESTS)

.PHONY: clean fclean

re: fclean
	$(MAKE) -s all

.PHONY: re

-include $(DEPS)
-include $(DEPS_DEBUG)
-include $(DEPS_TESTS)