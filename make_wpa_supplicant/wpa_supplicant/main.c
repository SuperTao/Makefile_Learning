#include <stdio.h>
#include "utils.h"
#include "common.h"

int main(int argc, char **argv)
{
	printf("hello world\n");

	utils_test();

	common_test();
	
	return 0;
}
