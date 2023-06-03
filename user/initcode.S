# Initial process that execs /init.
# This code runs in user space.

#include "syscall.h"

# exec(init, argv)
.globl start
start:
        la a0, init
        la a1, argv
        li a7, SYS_exec
        ecall

# for(;;) exit();
exit:
        li a7, SYS_exit
        ecall
        jal exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
  .long init
  .long 0

/**
用户代码将exec需要的参数放在寄存器a0和a1中，并将系统调用号放在a7中。
系统调用号与syscalls数组中的条目相匹配，syscalls数组是一个函数指针表（kernel/syscall.c:108）。
ecall指令陷入(trap)到内核中，执行uservec、usertrap和syscall，和我们之前看到的一样。
 */
