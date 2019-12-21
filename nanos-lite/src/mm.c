#include "memory.h"
#include "proc.h"

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *p = pf;
  pf += PGSIZE * nr_page;
  assert(pf < (void *)_heap.end);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk, intptr_t increment) {
  if (current->max_brk == 0) {
    current->max_brk = (brk & 0xfff) ? ((brk & ~0xfff)+PGSIZE) : brk;
    return 0;
  }
  if (current->max_brk < brk) {
    // printf("mm_brk work\nbrk: %d  max_brk: %d\n", brk, current->max_brk);
    uintptr_t va;
    void* pa;
    uintptr_t start;
    if ((current->max_brk & ~0xfff) == current->max_brk) start = current->max_brk;
    else start = (current->max_brk & ~0xfff) + PGSIZE;
    for (va = start; va < brk; va += PGSIZE) {
      // printf("one time\n");
      pa = new_page(1);
      _map(&current->as, (void*)va, pa, 1);
    }
    current->max_brk = brk;
    // printf("mm_brk success\n");
  }
  return 0;

}

void init_mm() {
  pf = (void *)PGROUNDUP((uintptr_t)_heap.start);
  Log("free physical pages starting from %p", pf);

  _vme_init(new_page, free_page);

  // Log("init_mm success");
}
