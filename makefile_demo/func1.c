#include <stdio.h>
#include "func1.h"

int func1(void)
{
	func1_test();
	return 0;
}

void func1_test(void)
{
	printf("%s, %s, %d\n", __FILE__, __func__, __LINE__);
}
