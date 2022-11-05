#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

int main() { 
  int pa[2], pb[2];
  //using two pipe to accomplish two-way communication
  if(pipe(pa)<0 || pipe(pb)<0){
    fprintf(STD_ERR, "pipe faild");
    exit(-1);
  }

  int pid = fork();
  if(pid == 0){
    //read parent's message
    char buff[100];
    close(pa[1]);
    int ret = read(pa[0], buff, sizeof(buff));
    if(ret == -1){
      fprintf(STD_ERR, "%d: read faild\n", getpid());
    }
    fprintf(STD_IN, "%d: received ping\n",getpid());
    close(pa[0]);
    //send data to parent
    close(pb[0]);
    ret = write(pb[1], "ball",5);
    if(ret < 0){
      fprintf(STD_ERR,"%d: write faild\n", getpid());
    }
    close(pb[1]);
  }else {
    char buff[100];
    //send data to child
    close(pa[0]); 
    int ret = write(pa[1], "ball",5);
    if(ret < 0 ){
      fprintf(STD_ERR, "%d: write faild\n", getpid());
    }
    close(pa[1]);
    //read child's message
    close(pb[1]);
    ret = read(pb[0], buff, sizeof(buff));
    if(ret < 0){
      fprintf(STD_ERR, "%d: read faild\n", getpid());
    }
    fprintf(STD_IN, "%d: received pong\n");
    close(pb[0]);
  }
  wait(0);
  exit(0); 
}