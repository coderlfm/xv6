// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run
{
  struct run *next;
};

struct
{
  struct spinlock lock;
  struct run *freelist;
} kmem;

void kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void *)PHYSTOP); // 初始化空闲列表
}

void freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char *)PGROUNDUP((uint64)pa_start);
  for (; p + PGSIZE <= (char *)pa_end; p += PGSIZE)
    kfree(p); // 将每个页都进行初始化(每次会加 PGSIZE 4096)
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa) // 释放指定的物理内存页
{
  struct run *r;

  if (((uint64)pa % PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE); /* 将这块区域的内容全部设置成111111111, 为什么设置成1？将内存区域填充为 0 或 1 都可以用来检测悬空引用。在 xv6 中，将内存区域填充为 1 的原因是为了更容易地发现错误。因为在内存区域中填充了大量的二进制 1，如果存在悬空引用，那么这些二进制 1 可能会被误认为是有效数据，从而导致程序出现错误。因此，将内存区域填充为 1 可以更容易地发现这种错误。另外，将内存区域填充为 1 还有一个好处，就是可以更容易地发现内存泄漏。如果一个内存区域被释放后，其中的数据仍然存在，那么这些数据可能会被误认为是有效数据，从而导致内存泄漏。将内存区域填充为 1 可以更容易地发现这种情况，因为在内存区域中填充了大量的二进制 1，如果存在内存泄漏，那么这些二进制 1 可能会被误认为是有效数据，从而导致程序出现错误。总之，将内存区域填充为 1 可以更容易地发现悬空引用和内存泄漏等问题。 */

  r = (struct run *)pa;

  acquire(&kmem.lock); // 拿到锁
  r->next = kmem.freelist;
  kmem.freelist = r; // 将这个块前置，放在freelist的最前面
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if (r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if (r)
    memset((char *)r, 5, PGSIZE); // fill with junk   将其释放，填充成555555555555，增加敏感信息泄露的难度，设置成5并不是必须的，设置成其他的也可以
  return (void *)r;
}

// 获取空闲内存
uint64
get_free_mem()
{
  struct run *r;
  int count = 0;

  acquire(&kmem.lock);
  r = kmem.freelist; // 内核空闲列表

  while (r)
  {
    r = r->next;
    count++;
  }
  release(&kmem.lock);

  return count * PGSIZE;
}
