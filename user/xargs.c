#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

#define STR_LEN 64

int main(int argc, char *argv[]) {
  char *cmd = argv[1];          // the name of program
  char *param_list[MAXARG];     // the parameter list
  char block[64] = {0};         // Incoming data
  char buf[64] = {0}, *p = buf; // the buffer , the pointer point to buffer
  int offset = 0;               // Offset of the parameter position
  int count = 0;                // The index of the parameter list
  int k = 0;

  for (int i = 1; i < argc; ++i) {
    param_list[count++] = argv[i];
  }

  while ((k = read(STD_IN, block, sizeof(buf))) > 0) {
    for (int i = 0; i < k; ++i) {
      if (block[i] == ' ') {
        buf[i - offset] = 0;
        param_list[count++] = p;
        p = &buf[i - offset + 1];

      } else if (block[i] == '\n') {
        buf[i - offset] = 0;
        offset = 0;
        param_list[count++] = p;
        param_list[count] = 0;
        p = buf;
        count = argc - 1;
        if (fork() == 0) {
          exec(cmd, param_list);
        }
        wait((int *)0);
      } else {
        buf[i - offset] = block[i];
      }
    }
  }

  exit(0);
}