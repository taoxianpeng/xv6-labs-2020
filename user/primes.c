#include "kernel/types.h"
// #include "kernel/stat.h"
#include "user.h"

#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

void child_thread(int read_pipe) {
  int pp[2];
  pipe(pp);
  int pid = fork();
  if (pid == 0) {
    close(pp[1]);
    child_thread(pp[0]);
    close(pp[0]);
  } else {
    // deal left data
    int n, tmp;
    close(pp[0]);
    int ret = read(read_pipe, &n, sizeof(int));
    if (ret <= 0) {
      exit(0);
    }
    while (read(read_pipe, &tmp, sizeof(int)) != 0) {
      if (tmp % n != 0) {
        write(pp[1], &tmp, sizeof(int));
      }
    }
    fprintf(STD_OUT, "prime %d\n", n);
    close(pp[1]);
    wait(0);
  }
  exit(0);
}

int main() {
  int p[2];
  pipe(p);
  int pid = fork();
  if (pid == 0) {
    close(p[1]);
    child_thread(p[0]);
    close(p[0]);
  } else {
    close(p[0]);
    for (int i = 2; i <= 35; ++i) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}