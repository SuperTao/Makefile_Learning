# GNU automake

# hello world
在空目录里面创建以下文件.
- src/main.c
  hello的源码。
  ```c
  #include <config.h>
  #include <stdio.h>
  int
  main (void)
  {
      puts ("Hello World!");
      puts ("This is " PACKAGE_STRING ".");
      return 0;
  }
  ```
- README
  ```
  This is a demonstration package for GNU Automake.
  Type ’info Automake’ to read the Automake manual.
  ```
- src/Makefile.am
  ```
  bin_PROGRAMS = hello
  hello_SOURCES = main.c
  ```
- Makefile.am
  ```
  AC_INIT([amhello], [1.0], [bug-automake@gnu.org])
  AM_INIT_AUTOMAKE([-Wall -Werror foreign])
  AC_PROG_CC
  AC_CONFIG_HEADERS([config.h])
  AC_CONFIG_FILES([
      Makefile
      src/Makefile
  ])
  AC_OUTPUT
  ```
- configure.ac
  Autoconf命令，用于创建configure脚本时使用
  ```
  AC_INIT([amhello], [1.0], [bug-automake@gnu.org])
  AM_INIT_AUTOMAKE([-Wall -Werror foreign])
  AC_PROG_CC
  AC_CONFIG_HEADERS([config.h])
  AC_CONFIG_FILES([
  	Makefile
	src/Makefile
  ])
  AC_OUTPUT
```

# auforeconf --install
使用Autotools创建编译系统的实例。`autoreconf --install`	
```
tao@T:~/temp/hello$ autoreconf --install
configure.ac:3: installing './compile'
configure.ac:2: installing './install-sh'
configure.ac:2: installing './missing'
src/Makefile.am: installing './depcomp'
```
除了compile,install-sh,missing,depcomp文件以外。
autoconf生成了另外的4个文件,configure, config.h.in, Makefile.in, src/Makefile.in。
后面三个文件是configure命令用于生成config.h, Makefile, src/Makefile的模板文件。

# 运行configure
```
tao@T:~/temp/hello$ ./configure
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a race-free mkdir -p... /usr/bin/mkdir -p
checking for gawk... no
checking for mawk... mawk
checking whether make sets $(MAKE)... yes
checking whether make supports nested variables... yes
checking for gcc... gcc
checking whether the C compiler works... yes
checking for C compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether the compiler supports GNU C... yes
checking whether gcc accepts -g... yes
checking for gcc option to enable C11 features... none needed
checking whether gcc understands -c and -o together... yes
checking whether make supports the include directive... yes (GNU style)
	checking dependency style of gcc... gcc3
	checking that generated files are newer than configure... done
	configure: creating ./config.status
	config.status: creating Makefile
	config.status: creating src/Makefile
	config.status: creating config.h
	config.status: executing depfiles commands
```
Makefile, src/Makefile, config.h文件被创建。

# make
```
tao@T:~/temp/hello$ make
make  all-recursive
make[1]: Entering directory '/home/tao/temp/hello'
Making all in src
make[2]: Entering directory '/home/tao/temp/hello/src'
make[2]: Nothing to be done for 'all'.
make[2]: Leaving directory '/home/tao/temp/hello/src'
make[2]: Entering directory '/home/tao/temp/hello'
make[2]: Leaving directory '/home/tao/temp/hello'
make[1]: Leaving directory '/home/tao/temp/hello'
tao@T:~/temp/hello$ src/hello 
Hello World!
This is amhello 1.0.
tao@T:~/temp/hello$ make distcheck
...
=============================================
amhello-1.0 archives ready for distribution: 
amhello-1.0.tar.gz
=============================================
```
- autoreconf命令只是在GNU Build System不存在的时候运行，后面修改Makefile.am或者configure.ac，只需要运行make即可。
- autoreconf是一个脚本,按顺序调用autoconf和automake以及其他的一系列命令。
- autoconf通过configure.ac创建configure
- automake通过一系列的Makefile.am和configure.ac创建各种各样的Makefile.in

# configure.ac
autoconf和automake都会读取这个文件。
- AC_开头的是Autoconf宏
- AM_开头的是Automake宏
- AC_INIT([amhello], [1.0], [bug-automake@gnu.org])
  包名，版本号，bug联系地址
- AM_INIT_AUTOMAKE([-Wall -Werror foreign])
  automake的选项，-Wall -Werror与编译器调用的选项无关,虽然选项的名称类似
  foreign通知automake，这个包不会遵守GNU的标准
- AC_PROG_CC
  让configure寻找C编译器，并设置为CC的值
- AC_CONFIG_HEADERS([config.h])
  让configure脚本去创建config.h，通过configure.ac中定义的一些宏，例如AC_INIT。
- AC_CONFIG_FILES
  AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
  通知configure通过对应的.in模板文件，生成新的文件
- AC_OUTPUT
  生成脚本，创建注册在AC_CONFIG_HEADERS and AC_CONFIG_FILES的文件.
