#include "test.h"
#include <stdio.h>
#include <stdlib.h>

enum STATE {
  dev_setup_test,
  compile_test,
  error_test,
};

void test_print(char *string) {
  printf(BLU);
  printf("Running %s...\n", string);
  printf(reset);
}

int test_run(char **argv) {
  printf(BLU);
  printf("Running test kit...\n");
  printf(reset);

  return 0;
}

int main(int argc, char **argv) {
  enum STATE state;

  test_run(argv);

  state = dev_setup_test;

  switch (state) {
  case dev_setup_test:
#ifdef __unix__ /* Unix to include MacOS with Linux */
    printf(reset);
    system("../scripts/dev-setup.sh");
    if (!system("../scripts/dev-setup.sh")) {
      printf(RED);
      printf("Dev setup script ERROR!\n");
      printf(reset);
      state = error_test;
    }
#endif
    state = compile_test;
    break;
  case compile_test:
    test_print("Running compile test\n");
    system("cd .. && make");
    if (!system("cd .. && make")) {
      printf(RED);
      printf("Compile test ERROR!\n");
      printf(reset);
      state = error_test;
    }
    break;
  case error_test:
    printf(RED);
    printf("Test kit failed!\n");
    printf(reset);
    return 1;
  }
  return 0;
}