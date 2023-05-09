#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"

void testFork() {
  // fork()在父进程中返回子进程的PID
  // 在子进程中返回0
  int pid = fork();
  if (pid > 0){
    printf("parent: child=%d\n", pid);
    pid = wait((int *)0);
    printf("child %d is done\n", pid);
  }
  else if (pid == 0) {
    printf("child: exiting\n");
    exit(0); // 通常0 表示成功，1表示失败
  }
  else {
    printf("fork error\n");
  }

}

void testExec() {
  char* argv[3];
  argv[0] = "echo";
  argv[1] = "hello";
  argv[2] = 0;
  execv("/bin/echo", argv);

}

int main(){
  testExec();
  return 0;
}
