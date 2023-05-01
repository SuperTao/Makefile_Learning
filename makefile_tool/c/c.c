#include <stdio.h>

int func_c(void)
{
	printf("%s\n", __func__);
	return 0;
}
