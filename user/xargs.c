
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

  // if(strlen(argv) > MAXARG) {
  //   fprintf(2, "%s 参数过长", argv);
  //   exit(1);
  // }

  char *buf[MAXARG] = { " " }, *p;
  char temp;
  char *comd = argv[1]; 

  p = buf[1];

  // 当标准输入关闭时，read会返回0
  while(read(0, &temp, 1) != 0) {

    if(&temp == '\n'){
      *++p = temp;
      continue;
    } else {
      *++p = ' ';
    }

    int pid;
    if((pid = fork()) < 0){
      fprintf(2, "xargs fork 子进程失败");
      close(0);
      exit(1);
    }

    // 子进程
    if(pid == 0) {
      exec(comd, buf);
    } else {
      wait(0);
      p = buf[1];
    }
  }

  return 0;
}
