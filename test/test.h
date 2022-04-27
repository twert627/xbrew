#ifndef TEST_H
#define TEST_H

#include <stdio.h>

#define RED "\e[0;31m"
#define BLU "\e[0;34m"
#define reset "\e[0m"

int test_run(char **argv);

void test_print(char *string);


#endif