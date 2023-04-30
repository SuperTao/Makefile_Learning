#include <stdio.h>
#include "func.h"

int func(void)
{
	func_test();
	return 0;
}

void func_test(void)
{
	printf("%s, %s, %d\n", __FILE__, __func__, __LINE__);
}
