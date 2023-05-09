#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    fprintf(2, "请输入睡眠的时间\n");
    exit(1);
  }

  int count = atoi(argv[1]);
  
  if(sleep(count) != 0) {
    fprintf(2, "sleep 系统调用 发生错误");
  }

  exit(0);
  return 0;
}
