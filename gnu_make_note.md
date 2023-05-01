### 3.6 Overriding Part of Another Makefile
创建一个通用的匹配规则，当Makefile中，没有target规则，会使用通配符规则进行。
```
foo:
	@echo "foo"

%: force
	@echo $@

force:;
```
- force的作用，防止当前目录下有和目标文件同名。另外保证每次make，目标都会重新编译。
- force:设置为空，覆盖以前的规则，防止搜索隐含的force规则进行编译。
测试输出。

```
tao@T:~/temp$ make foo
Makefile
foo
tao@T:~/temp$ make hello
Makefile
hello
tao@T:~/temp$ make test
Makefile
test
```

### 3.9 Secondary Expansion
二次展开,对依赖进行二次展开。
```
For example, consider this makefile:
	.SECONDEXPANSION:
	ONEVAR = onefile
	TWOVAR = twofile
	myfile: $(ONEVAR) $$(TWOVAR)
After the first expansion phase the prerequisites list of the myfile target will be onefile
and $(TWOVAR); the first (unescaped) variable reference to ONEVAR is expanded, while
the second (escaped) variable reference is simply unescaped, without being recognized as a
variable reference. Now during the secondary expansion the first word is expanded again
but since it contains no variable or function references it remains the value onefile, while
the second word is now a normal reference to the variable TWOVAR, which is expanded
to the value twofile. The final result is that there are two prerequisites, onefile and
twofile.
Obviously, this is not a very interesting case since the same result could more easily have
been achieved simply by having both variables appear, unescaped, in the prerequisites list.
One difference becomes apparent if the variables are reset; consider this example:
	.SECONDEXPANSION:
	AVAR = top
	onefile: $(AVAR)
	twofile: $$(AVAR)
	AVAR = bottom
Here the prerequisite of onefile will be expanded immediately, and resolve to the value
top, while the prerequisite of twofile will not be full expanded until the secondary expansion
and yield a value of bottom.
```

### 4.4.6 Directory Search for Link Libraries
prerequisite里面有库文件，编译时会去系统目录里面查找对应的libxxx.so或者libxxx.a
```
foo : foo.c -lcurses
	cc $^ -o $@
```
make foo时，将会去查找libcurses.so或者libcurse.a库。

### 4.5 Phony Targets
- 并行编译，设置目标的依赖关系，解决编译的报错，让一些目标优先编译。

```
SUBDIRS = foo bar baz
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

foo: baz
```
最后这一行，先编译baz目录的内容，然后再编译foo的内容。
- 伪目标依赖

```
.PHONY: cleanall cleanobj cleandiff
cleanall : cleanobj cleandiff
	rm program
cleanobj :
	rm *.o
cleandiff :
	rm *.diff
```
`make cleanall`将会执行cleanobj cleandiff`

### 4.13 Generating Prerequisites Automatically
自动产生依赖头文件,以免编译的时候在Makefile中手动添加头文件依赖关系
- -M选项，生成
- -MD选项，将输出内容保存到.d文件中,可以自定义文件内容
- -Wp,MD,xxx.d，将输出内容保存在自定义文件中

```c
#include <stdio.h>
int main(void)
{
	return 1;
}
```
```
tao@T:~/temp$ gcc -M main.c
main.o: main.c /usr/include/stdc-predef.h /usr/include/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/features.h /usr/include/features-time64.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h \
 /usr/lib/gcc/x86_64-linux-gnu/11/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/floatn.h \
 /usr/include/x86_64-linux-gnu/bits/floatn-common.h
```

### 5.3 Recipe Execution
recipe执行的时候，每一行命令都会调用一个sub-shell。sub-shell相当于一个子进程或者子线程，所以每一条命令是相互独立的，没有影响。
例如在上一条命令执行cd，下一条命令不会受到cd命令的影响。
```
all:
	cd dir
	echo 111 > test
```
dir目录里面不会生成test文件,在当前目录生成test文件。
```
all:
	cd dir;echo 111 > test
```
dir目录里面生成test文件。

```
all:
	cd dir && echo 111 > test
```
cd命令失败，将不会执行后面的命令。
执行成功，在dir目录生成test文件。

```
.ONESHELL:
all:
	cd dir
	echo 111 > test
```
使用`.ONESHELL:`设置之后，所有的命令在一个shell执行。
dir会生成test文件。

### 6.8 Defining Multi-Line Variables
define命令，定义多行变量
```
define two-lines =
	foo
	$(bar)
endef`
```
这样定义以后，每一行命令执行的时候，都是各自调用一个shell。

### 6.13 Suppressing Inheritance
CFLAGS设置成-g
```
prog : CFLAGS = -g
prog : prog.o foo.o bar.o
```
注意这个private的用法，下面的a.o和b.o就不会使用private定义的值
```
EXTRA_CFLAGS =
prog: private EXTRA_CFLAGS = -L/usr/local/lib
prog: a.o b.o
```

### 7.2 Syntax of Conditionals
- ifeq
`ifeq ($(strip $(foo)),)`
去掉变量里面的空格进行比较。

```
foo = test    #test后面有空格
ifeq ($(strip $(foo)), test)
#ifeq ($(foo), test)
	bar = hello
else
	bar = world
endif

all:
	@echo $(bar)

```
比较不限于使用括号"()", 还可以使用其他字符。
```
ifneq (arg1, arg2)
ifneq ’arg1’ ’arg2’
ifneq "arg1" "arg2"
ifneq "arg1" ’arg2’
ifneq ’arg1’ "arg2"
```
- ifdef
use ifeq ($(foo),). For example,

```
bar =
foo = $(bar)
ifdef foo
	frobozz = yes
else
	frobozz = no
endif
```
sets ‘frobozz’ to ‘yes’, while:

```
foo =
ifdef foo
	frobozz = yes
else
	frobozz = no
endif
```
sets ‘frobozz’ to ‘no’.

### 8.2 Functions for String Substitution and Analysis
- patsubst函数
`$(patsubst pattern,replacement,text)`
使用$(var:pattern=replacement)的方式替换$(patsubst)函数

```
Substitution references (see Section 6.3.1 [Substitution References], page 69)
are a simpler way to get the effect of the patsubst function:
	$(var:pattern=replacement)
is equivalent to
	$(patsubst pattern,replacement,$(var))

The second shorthand simplifies one of the most common uses of patsubst:
replacing the suffix at the end of file names.
	$(var:suffix=replacement)
is equivalent to
	$(patsubst %suffix,%replacement,$(var))
For example, you might have a list of object files:
objects = foo.o bar.o baz.o
To get the list of corresponding source files, you could simply write:
	$(objects:.o=.c)
instead of using the general form:
	$(patsubst %.o,%.c,$(objects))
```
- filter
`$(filter pattern...,text)`
编译的时候，直接在recipe里面调用filter函数，生成文件。

```
sources := foo.c bar.c baz.s ugh.h
foo: $(sources)
	cc $(filter %.c %.s,$(sources)) -o foo
```
也可以将filter生成的结果赋值给变量，然后在recipe里面使用。
	
- CFLAGS -I

```
First, the subst function is used to change the colons to spaces:
	$(subst :, ,$(VPATH))
This produces ‘src ../headers’. Then patsubst is used to turn each directory name
into a ‘-I’ flag. These can be added to the value of the variable CFLAGS, which is passed
automatically to the C compiler, like this:
	override CFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH)))
The effect is to append the text ‘-Isrc -I../headers’ to the previously given value of
CFLAGS. The override directive is used so that the new value is assigned even if the
previous value of CFLAGS was specified with a command argument (see Section 6.7 [The
override Directive], page 75).
```
以前用CFLAGS的时候，总是会采用
```
CFLAGS += -Idir1
CFLAGS += -Idir2
CFLAGS += -Idir3
```
这里直接使用patsubst函数将整个字符串都自动加上-I。
`override CFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH)))`

### 8.6 The foreach Function
```
find_files = $(wildcard $(dir)/*)
dirs := a b c d
files := $(foreach dir,$(dirs),$(find_files))
```
'='延时变量赋值,所以这里find_files先使用，后面再赋值

### 8.7 The file Function
```
all:
	$(file > test,hello)
	$(file >> test,world)
	@echo $(file < test1)
```

### 8.8 The call Function
`$(call variable,param,param,...)`
```
When make expands this function, it assigns each param to temporary variables $(1),
$(2), etc. The variable $(0) will contain variable. There is no maximum number of
parameter arguments. There is no minimum, either, but it doesn’t make sense to use call
with no parameters.
```
```
This macro simply reverses its arguments:
	reverse = $(2) $(1)
	foo = $(call reverse,a,b)
Here foo will contain ‘b a’.
```

### 8.9 The value Function
```
FOO = $PATH
all:
	@echo $(FOO)
	@echo $(value FOO)
```
输出结果
```
ATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/home/tao/tools/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf/bin
```

### 8.10 The eval Function
函数原型 $(eval text)
它的意思是 text 的内容将作为makefile的一部分而被make解析和执行。
```
define func
$1:
	cc $2 -o $$@
endef

$(eval $(call func,main,main.c))
#$(info $(call func,main,main.c))
```
输出结果
`cc main.c -o main`
如果采用info执行，将会报错，没有target
`$(info $(call func,main,main.c))`
```
main:
	cc main.c -o $@
	make: *** No targets.  Stop.
```

### 8.13 Functions That Control Make
- error,致命错误,打印行号,退出
  `$(error text...)`
- warning,会打印行号,不退出
  `$(warning text...)
- info
  `$(info text...)`

### 8.14 The shell Function
makefile中export的变量，将会传给shell。引用时需要用`$$`
shell中export的变量,makefile进行引用，需要用两个`$$`。
例如在shell运行如下命令.
`export TEST=hello world`
Makefile如下.
```
export HI = $(shell echo hi)
all: 
	@echo $$HI
	@echo $$TEST
```
输出结果
```
hi
hello world
```

### 10.3 Variables Used by Implicit Rules
`make -p 查看make的默认变量和规则`
