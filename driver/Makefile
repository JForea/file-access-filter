PROJECT_NAME := file-access-filter

ifneq ($(KERNELRELEASE),)

obj-m += $(PROJECT_NAME).o
file-access-filter-y := src/main.o src/device_handlers.o src/rule_list.o

else

KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
BUILD_DIR := $(CURDIR)/build

CC ?= gcc
CFLAGS := -Wall -Wextra -Werror -Isrc
LDLIBS := -lcunit

USER_TEST_BIN := $(BUILD_DIR)/user
USER_TEST_SRC := tests/user/helpers-tests.c

HELPERS  := src/helpers.c

.PHONY: module test load unload clean

module:
	mkdir -p $(BUILD_DIR)
	$(MAKE) -C $(KERNEL_DIR) M=$(CURDIR) MO=$(BUILD_DIR) modules

test: $(USER_TEST_BIN)
	$(USER_TEST_BIN)

$(USER_TEST_BIN): $(HELPERS) $(USER_TEST_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(HELPERS) $(USER_TEST_SRC) $(LDLIBS) -o $@

load: module
	sudo insmod $(BUILD_DIR)/$(PROJECT_NAME).ko

unload:
	sudo rmmod $(PROJECT_NAME)

clean:
	rm -rf $(BUILD_DIR)

endif