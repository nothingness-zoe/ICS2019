#include "nemu.h"

paddr_t page_translate(vaddr_t addr) {
  paddr_t paddr = addr;
  if(cpu.cr0.paging) {
    printf("paging: %d\n", cpu.cr0.paging);
    // uintptr_t pde_addr = (uintptr_t)((cpu.cr3.page_directory_base << 12) + (((addr>>22)&0xfff)<<2));
    PDE* pde_p = (PDE*)(uintptr_t)(cpu.cr3.page_directory_base << 12);
    PDE pde;
    pde.val = paddr_read((uintptr_t)&pde_p[addr>>22], 4);
    Assert(pde.present, "addr: %#x", addr);
    uintptr_t pte_addr = (uintptr_t)((pde.page_frame<<12)+ ((addr<<10)>>22));
    PTE pte;
    pte.val = paddr_read(pte_addr, 4);
    Assert(pte.present, "addr: %#x", addr);
    paddr = (pte.page_frame<<12)+(addr&0xfff);
  }
  return paddr;
}

uint32_t isa_vaddr_read(vaddr_t addr, int len) {
  if (PAGE_SIZE - (addr & 0xfff) < len) {
    /* this is a special case, you can handle it later. */
    assert(0);
  }
  else {
    paddr_t paddr = page_translate(addr);
    return paddr_read(paddr, len);
  }
}

void isa_vaddr_write(vaddr_t addr, uint32_t data, int len) {
  if (PAGE_SIZE - (addr & 0xfff) < len) {
    /* this is a special case, you can handle it later. */
    assert(0);
  }
  else {
    paddr_t paddr = page_translate(addr);
    return paddr_write(paddr, data, len);
  }
}
