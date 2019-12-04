#include "common.h"
#include "syscall.h"

uintptr_t sys_write(uintptr_t fd, void* buf, uintptr_t count) {
  // _putc('w'); _putc('\n');
  uintptr_t count_write = 0;
  if (fd == 1 || fd == 2) {
    char * out = (char *)buf;
    // while (*out && count--) {
    //   _putc(*out);
    //   out++;
    //   count_write++;
    // }
    for (int i=0; i < count; i++) {
      _putc(out[i]);
    }
    count_write = count;
  }
  Log("1 time");
  return count_write;
}

void sys_execve(const char * filename) {
  naive_uload(NULL, filename);
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    // case SYS_exit: _halt(a[1]); break;
    case SYS_exit: sys_execve("/bin/init"); break;
    case SYS_yield: _yield(); c->GPRx = 0; break;
    case SYS_write: c->GPRx = fs_write(a[1], (void*)a[2], a[3]); break;
    case SYS_brk: c->GPRx = 0; break; // TODO: sys_brk()
    case SYS_execve: sys_execve((char*)a[1]); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
