# 通用Makefile
整理一个通用的Makefile,方便后面开发使用。
### 目录结构
```
.
├── c
│   ├── c.c
│   ├── c.h
│   └── Makefile
├── d
│   ├── d.c
│   ├── dd
│   │   ├── dd.c
│   │   ├── dd.h
│   │   └── Makefile
│   ├── d.h
│   └── Makefile
├── main.c
├── Makefile
├── Makefile.build
└── README.md
```
### Makefile分类
- 顶层目录的Makefile
  - 设置编译工具,可执行文件,子目录。
- 顶层目录的Makefile.build
  - 通用makefile,每个编译都会调用
- 子目录的Makefile
  - `obj-y += file.o` 表示把当前目录下的file.c编进程序里，
  - `obj-y += subdir/` 表示要进入subdir这个子目录下去寻找文件来编进程序里，是哪些文件由subdir目录下的Makefile决定。
  - `EXTRA_CFLAGS` 它给当前目录下的所有文件(不含其下的子目录)设置额外的编译选项, 可以不设置
  - `CFLAGS_xxx.o` 它给当前目录下的xxx.c设置它自己的编译选项, 可以不设置
  - 子目录最后的'/'不能省略

### Makefile.build分析
```Makefile
PHONY := __build
__build:

obj-y :=
subdir-y :=
EXTRA_CFLAGS :=

include Makefile

# 获取子目录的名称,并去掉目录名称最后的‘/’
__subdir-y	:= $(patsubst %/,%,$(filter %/, $(obj-y)))
subdir-y	+= $(__subdir-y)
# 获取每个子目录里面的built-in.o文件
subdir_objs := $(foreach f,$(subdir-y),$(f)/built-in.o)

# 查找makefile里面的文件,不包含目录
cur_objs := $(filter-out %/, $(obj-y))
# 获取头文件的依赖关系文件
dep_files := $(foreach f,$(cur_objs),.$(f).d)
# 查找当前目录的文件是否存在
dep_files := $(wildcard $(dep_files))

ifneq ($(dep_files),)
  include $(dep_files)
endif

PHONY += $(subdir-y)

__build: $(subdir-y) built-in.o

$(subdir-y):
	make -C $@ -f $(TOP_DIR)/Makefile.build

# 生成built-in.o,由当前目录的.o文件和子目录的built-in.o文件共同生成
# LD 代表交叉编译器的链接器; -r 选项代表可重定位的输出, 一个输出文件可再次作为ld输入
# -o 设置输出文件; $@ 目标名称; $^ 所有不重复的依赖文件
built-in.o : $(cur_objs) $(subdir_objs)
	$(LD) -r -o $@ $^
# 头文件依赖关系
dep_file = .$@.d

%.o : %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -Wp,-MD,$(dep_file) -c -o $@ $<
	
.PHONY : $(PHONY)
```

### Makefile分析
```Makefile
# 设置交叉编译工具
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
# 到处符号表
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
# 可执行文件名称
TARGET := main
# 源文件以及子目录
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
```

### 存在问题
- 使用make -j命令,设置多个线程编译时可能需要设置依赖关系
