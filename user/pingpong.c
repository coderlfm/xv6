
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{

  int p1[2]; // 管道

  if(pipe(p1) == -1) {
    fprintf(2,"pipe(fd) 发生错误");
  }

  if (fork() == 0)
  {
    char buffer[1];
    read(p1[0], buffer, 1);
    close(p1[0]);
    fprintf(1, "%d: received ping\n", getpid());
    write(p1[1], buffer, 1); // 向管道写入
    close(p1[1]);
  }
  else
  {
    char buffer[1] ;
    buffer[0] = 'a';
    write(p1[1], buffer, 1); // 向管道写入
    close(p1[1]);
    read(p1[0], buffer, 1); // 读取
    fprintf(0, "%d: received pong\n", getpid());
    close(p1[0]);
  }
  exit(0);

  return 0;
}
