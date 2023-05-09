void main() {
  int p[2];
  char *argv[2];
  argv[0] = "wc";
  argv[1] = 0;
  pipe(p);
  if (fork() == 0) {
      close(0);
      dup(p[0]);
      close(p[0]);
      close(p[1]);
      
      // wc程序中使用标准输入0，所以在此之前我们将管道的输入复制一份，让文件描述符0也可以访问到管道的输入，同时在wc程序执行之后，应该将文件描述符0关闭掉
      exec("/bin/wc", argv);
  } else {
      close(p[0]);
      write(p[1], "hello world\n", 12);
      close(p[1]);
  }

}
