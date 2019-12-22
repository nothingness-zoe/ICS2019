#include "proc.h"
#include <elf.h>
#include <fs.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
// # define DEFAULT_ENTRY 0x3000000

uintptr_t heapstart;
static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
  // printf("remdisk_size: %x\n", get_ramdisk_size());
  //uintptr_t DEFAULT_ENTRY = 0x3000000;
  // ramdisk_read((void *)DEFAULT_ENTRY, 0x1000, get_ramdisk_size()-0x1000);
  // ramdisk_read((void *)DEFAULT_ENTRY, 0x1000, 0x28f3-0x1000);
  // printf("entry: %x\n", (uintptr_t)DEFAULT_ENTRY);
  // printf("success\n");
  // return (uintptr_t)DEFAULT_ENTRY;

  // Elf_Ehdr ehdr;
  // ramdisk_read(&ehdr, 0, sizeof(ehdr));

  // for (int i=0; i < ehdr.e_phnum; i++) {
  //   Elf_Phdr phdr;
  //   ramdisk_read(&phdr, ehdr.e_phoff + i*ehdr.e_phentsize, ehdr.e_phentsize);
  //   if (phdr.p_type == PT_LOAD) {
  //     ramdisk_read((void*)phdr.p_vaddr, phdr.p_offset, phdr.p_filesz);
  //     memset((void*)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
  //   }
  // }
  
  // return ehdr.e_entry;

  // Elf_Ehdr ehdr;
  // ramdisk_read(&ehdr, 0, sizeof(ehdr));
  // // printf("e_phnum: %d\n", ehdr.e_phnum);

  // // for (int i=0; i < ehdr.e_phnum; i++) {
  // Elf_Phdr phdr;
  // int fd = fs_open(filename, 0, 0);
  // size_t size = fs_filesz(fd);
  // fs_read(fd, &phdr.p_vaddr, size);

  // return ehdr.e_entry;

  // int fd = fs_open(filename, 0, 0);
  // // printf("open: %d\n", fd);
  // size_t size = fs_filesz(fd);
  // // printf("size:%d\n", size);
  // fs_read(fd, (void*)DEFAULT_ENTRY, size);
  // // printf("read\n");
  // fs_close(fd);
  // return DEFAULT_ENTRY;

  Elf_Ehdr ehdr;
  int fd = fs_open(filename, 0, 0);
  fs_read(fd, &ehdr, sizeof(ehdr));

  // for (int i=0; i < ehdr.e_phnum; i++) {
  //   Elf_Phdr phdr;
  //   fs_lseek(fd, ehdr.e_phoff + i*ehdr.e_phentsize, SEEK_SET);
  //   fs_read(fd, &phdr, ehdr.e_phentsize);
  //   if (phdr.p_type == PT_LOAD) {
  //     fs_lseek(fd, phdr.p_offset, SEEK_SET);      
  //     fs_read(fd, (void*)phdr.p_vaddr, phdr.p_filesz);
  //     // fs_close(fd);
  //     memset((void*)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz);
  //     // break;
  //   }
  // }

  // int nr_page = 0;
  for (int i=0; i < ehdr.e_phnum; i++) {
    Elf_Phdr phdr;
    fs_lseek(fd, ehdr.e_phoff + i*ehdr.e_phentsize, SEEK_SET);
    fs_read(fd, &phdr, ehdr.e_phentsize);
    if (phdr.p_type == PT_LOAD) {
      fs_lseek(fd, phdr.p_offset, SEEK_SET);      
      uint32_t offset = 0;
      for (; offset < phdr.p_memsz ; offset += PGSIZE) {
        void* pa = new_page(1);
        // nr_page++;
        _map(&(pcb->as), (void*)(phdr.p_vaddr + offset), pa, 1);
        uint32_t len = phdr.p_filesz - offset >= PGSIZE ? PGSIZE : phdr.p_filesz - offset;
        fs_read(fd, pa, len);
        memset((void*)(pa+len), 0, PGSIZE - len);
      }
      if (phdr.p_memsz - phdr.p_filesz/PGSIZE >PGSIZE) {
        for (; offset < phdr.p_memsz; offset += PGSIZE) {
          void* pa = new_page(1);
          // nr_page++;
          _map(&(pcb->as), (void*)(phdr.p_vaddr + offset), pa, 1);
          memset(pa, 0, PGSIZE);
        }
      }
      // heapstart = ehdr.e_entry + phdr.p_memsz;
      // heapstart = ehdr.e_entry;
    }
  }

  fs_close(fd);
  printf("e_entry: %p\n", ehdr.e_entry);
  printf("pcb->as: %p\n", pcb->as);
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  //printf("entry: %x\n", entry);
  ((void(*)())entry)();
  //printf("naive_uload success\n");
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  _protect(&(pcb->as));
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
