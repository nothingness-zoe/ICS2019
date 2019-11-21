#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
# define DEFAULT_ENTRY 0x3000000

static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
  // printf("remdisk_size: %x\n", get_ramdisk_size());
  //uintptr_t DEFAULT_ENTRY = 0x3000000;
  ramdisk_read((void *)DEFAULT_ENTRY, 0x1000, get_ramdisk_size()-0x1000);
  // ramdisk_read((void *)DEFAULT_ENTRY, 0x1000, 0x28f3-0x1000);
  // printf("entry: %x\n", (uintptr_t)DEFAULT_ENTRY);
  // printf("success\n");
  return (uintptr_t)DEFAULT_ENTRY;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  printf("entry: %x\n", entry);
  ((void(*)())entry)();
  printf("naive_uload success\n");
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
