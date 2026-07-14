CC ?= gcc
CFLAGS := -Wall -Wextra -Werror -Isrc
LDLIBS := -lcunit

BUILD_DIR := $(CURDIR)/build

USER_TEST_BIN := $(BUILD_DIR)/user
USER_TEST_SRC := tests/user/helpers-tests.c

HELPERS  := src/helpers.c

.PHONY: test clean

test: $(USER_TEST_BIN)
	$(USER_TEST_BIN)

$(USER_TEST_BIN): $(HELPERS) $(USER_TEST_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(HELPERS) $(USER_TEST_SRC) $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)
