#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int isPrime(int num) {
  if (num <= 1) 
    return 0;

  for (int i = 2; i < num; i++) {
    if ((num % i) == 0) 
      return 0;
  }

  return 1;
}

int main() {
  int p[2];

  if(pipe(p) == -1){
    fprintf(2, "创建管道发生错误");
    exit(1);
  }
  
  for(int i = 2; i<= 35; i++) {
    write(p[1], &i, sizeof i);

    if(isPrime(i) == 1) {
      int pid = fork();

      if(pid < 0) {
        fprintf(2, "创建子进程错误");
        exit(1);
      }

      if(pid == 0) {
        int num;
        do {
          read(p[0], &num, sizeof num);
        } while (isPrime(num) == 0);
        
        fprintf(1, "prime %d \n", num);
        close(p[0]);
        exit(0);
      } else {
        wait(0);
      }
    }
  }
  close(p[1]);
  exit(0);
}
