#include "proc.h"

#define MAX_NR_PROC 4

void naive_uload(PCB *pcb, const char *filename);
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite for the %dth time!", j);
    j ++;
    _yield();
    // printf("once\n");
  }
}

void init_proc() {
  Log("Initializing processes...");

  // context_kload(&pcb[0], (void*)hello_fun);
  // context_uload(&pcb[1], "/bin/init");
  context_uload(&pcb[0], "/bin/hello");
  context_uload(&pcb[1], "/bin/pal");
  context_uload(&pcb[2], "/bin/pal");
  context_uload(&pcb[3], "/bin/pal");
  switch_boot_pcb();

  // load program here
  // naive_uload(NULL, "/bin/init");
  // Log("Finish initializing processes...");
}

static int count = 0;
extern int fg_pcb;
_Context* schedule(_Context *prev) {
  // return NULL;
  current->cp = prev;

  // current = &pcb[0];
  if (count == 0) {
    current = &pcb[0];
  }
  else {
    // current = &pcb[1];
    current = &pcb[fg_pcb];
  }
  count = (count+1)%1000;
  return current->cp;
}

void sys_execve(const char * filename) {
  naive_uload(NULL, filename);
}
