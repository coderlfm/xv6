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

struct run {
  struct run *next;  // 空闲列表的item
};

struct //  // 空闲列表受自旋锁的保护，整个列表使用一把大锁来保护 
{
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");    // 初始化加锁
  freerange(end, (void *)PHYSTOP); // 初始化空闲列表，从内核开始到PHYSTOP的结束之间的每一页
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,  释放内存
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r; // 将这个空闲块前置，放在freelist的最前面
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.  分配一个4096大小的内存
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);  // 给空闲列表上锁，随后取出一个空闲的内存块
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if (r)
    memset((char *)r, 5, PGSIZE); // fill with junk   将这个空闲块的数据填充成555555555555，增加敏感信息泄露的难度，设置成5并不是必须的，设置成其他的也可以
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
