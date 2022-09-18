CC        ?= gcc
TARGET    := xbrew-test
BUILD_DIR := ./test-build
SRC_DIRS  := ./

SRCS      := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s')
OBJS 	  	:= $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS 	  	:= $(OBJS:.o=.d)
INC_DIRS  := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS  := $(INC_FLAGS) -MMD -MP -Wall -Wextra 

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)