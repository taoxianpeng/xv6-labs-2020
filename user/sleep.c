#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

int main(int argc, char *argv[]) {
  int sleeptime;
  if (argc != 2) {
    fprintf(STD_ERR, "arguments is illeage\n");
    exit(-1);
  }

  sleeptime = atoi(argv[1]);

  sleep(sleeptime);

  exit(0);
}
