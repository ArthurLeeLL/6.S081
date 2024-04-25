#include "param.h"
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

//
// This file contains copyin_new() and copyinstr_new(), the
// replacements for copyin and coyinstr in vm.c.
//

static struct stats {
  int ncopyin;
  int ncopyinstr;
} stats;

int
statscopyin(char *buf, int sz) {
  int n;
  n = snprintf(buf, sz, "copyin: %d\n", stats.ncopyin);
  n += snprintf(buf+n, sz, "copyinstr: %d\n", stats.ncopyinstr);
  return n;
}

// Copy from user to kernel.
// Copy len bytes to dst from virtual address srcva in a given page table.
// Return 0 on success, -1 on error.
int
copyin_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 len)
{
  uint64 n, va0, pa0;

  struct proc *p = myproc();

  if (srcva >= p->sz || srcva+len >= p->sz || srcva+len < srcva)
    return -1;

  while(len > 0){
      va0 = PGROUNDDOWN(srcva);
      pa0 = walkaddr(p->pagetable, va0);
      if(pa0 == 0)
          return -1;
      n = PGSIZE - (srcva - va0);
      if(n > len)
          n = len;
      memmove(dst, (void *)(pa0 + (srcva - va0)), n);
      len -= n;
      dst += n;
      srcva = va0 + PGSIZE;
  }

  memmove((void *) dst, (void *)srcva, len);
  stats.ncopyin++;   // XXX lock
  return 0;
}

// Copy a null-terminated string from user to kernel.
// Copy bytes to dst from virtual address srcva in a given page table,
// until a '\0', or max.
// Return 0 on success, -1 on error.
int
copyinstr_new(pagetable_t pagetable, char *dst, uint64 srcva, uint64 max)
{
    struct proc *cp = myproc();
//    char *s = (char *) srcva;
    stats.ncopyinstr++;   // XXX lock
//    for(int i = 0; i < max && srcva + i < p->sz; i++){
//        dst[i] = s[i];
//        if(s[i] == '\0')
//            return 0;
//    }
    uint64 n, va0, pa0;
    int got_null = 0;
    while(got_null == 0 && max > 0){
        va0 = PGROUNDDOWN(srcva);
        pa0 = walkaddr(cp->pagetable, va0);
        if(pa0 == 0)
            return -1;
        n = PGSIZE - (srcva - va0);
        if(n > max)
            n = max;
        char *p = (char *) (pa0 + (srcva - va0));
        while(n > 0){
            if(*p == '\0'){
                *dst = '\0';
                got_null = 1;
                break;
            } else {
                *dst = *p;
            }
            --n;
            --max;
            p++;
            dst++;
        }
        srcva = va0 + PGSIZE;
    }
    if(got_null){
        return 0;
    } else {
        return -1;
    }
}
