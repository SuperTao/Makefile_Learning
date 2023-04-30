# 设置交叉编译器的前缀
#CROSS_COMPILE=arm-none-linux-gnueabihf-

# 头文件路径加入到CFLAGS中
INCLUDE_DIR += ./include
CFLAGS = -I$(INCLUDE_DIR)

# main
MAIN = main
# 库文件
LIBFUNC = libfunc.a

# Makefile会默认编译第一个目标文件
# 这里使用了一个伪目标，会同时编译多个目标文件
all: $(MAIN) $(LIBFUNC)

# 源码文件，手动添加
SRC += main.c func.c func1.c

# 目标文件，.o通过.c替换生成
# 将SRC文件中，所有以.c结尾的文件，替换成.o结尾，并重新赋值给SRC
OBJ += $(SRC:%.c=%.o)

# 编译main
$(MAIN): $(OBJ)
	$(CROSS_COMPILE)gcc $(CFLAGS) -o $@ $^

# 打包生成libfunc.a
$(LIBFUNC): $(OBJ)
	$(CROSS_COMPILE)ar rc $@ $(OBJ)

# .o文件通过.c文件编译得到
%.o:%.c
	$(CROSS_COMPILE)gcc $(CFLAGS) -o $@ -c $<

# clean伪目标，用于删除
clean:
	rm -rf $(OBJ)
	rm -rf $(MAIN)
	rm -rf $(LIBFUNC)
