#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

struct stat {
  int dev;     // 文件系统的磁盘设备
  uint ino;    // Inode编号
  short type;  // 文件类型
  short nlink; // 指向文件的链接数
  uint64 size; // 文件字节数
};
