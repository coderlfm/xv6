#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *findName) {

  int fd;           // 文件描述符
  struct dirent de; // 目录信息
  struct stat st;   // 打开的文件信息

  // 获取文件描述符
  if((fd = open(path, 0)) < 0) {
    fprintf(2, "find: open 失败 %s", path);
    exit(1);
  }

  // 读取文件
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: stat 失败 %s", path);
    close(fd);
    return;
  }

  if(st.type != T_DIR) {
    fprintf(2, "%s 路径错误，请输入正确的路径", path);
    close(fd);
    return;
  }

  // 开始读取目录下的文件
  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    // 非正常目录
    if(de.inum == 0) continue;

    if((strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0)) continue;
    
    struct stat childst;   
    char childdir[512], *p;
    strcpy(childdir, path);
    p = childdir + strlen(childdir);
    *p++ = '/';
    memmove(p, de.name, DIRSIZ); // 拼接文件名 ./foo
      
    if(stat(childdir, &childst) < 0) {
      fprintf(2, "%s 读取发生错误", childdir);
      close(fd);
      exit(1);
    }

    switch (childst.type){
      case T_FILE:
          // 判断要查找的文件名是否一致
          if(strcmp(de.name, findName) == 0) {
            fprintf(1, "%s\n", childdir);
            continue;
          }
        break;

      case T_DIR:
          find(childdir, findName);
        break;
      
      default:
          continue;
        break;
    }
  }
}

int main(int argc, char *argv[]) {

  if(argc < 3) {
    fprintf(2, "请输入需要查询的目录以及内容。如 find . b");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
  return 0;
  
}
