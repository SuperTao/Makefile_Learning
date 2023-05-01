CROSS_COMPILE = 
AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CC) -E
CXX     = $(CROSS_COMPILE)g++
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

export AS LD CC CPP CXX AR NM
export STRIP OBJCOPY OBJDUMP

TOP_DIR := $(shell pwd)
export TOP_DIR

CFLAGS := -Wall -O2 -g
CFLAGS += -I $(TOP_DIR)/c
CFLAGS += -I $(TOP_DIR)/d
CFLAGS += -I $(TOP_DIR)/d/dd

LDFLAGS := 

export CFLAGS LDFLAGS

TARGET := main

obj-y += main.o
obj-y += c/
obj-y += d/

all: start_recursive_build $(TARGET) 
	@echo $(TARGET) has been built!

start_recursive_build:
	make -C ./ -f $(TOP_DIR)/Makefile.build

$(TARGET): built-in.o
	$(CC) -o $(TARGET) $^ $(LDFLAGS)

clean:
	rm -f $(shell find -name "*.[od]")
	rm -f $(TARGET)
