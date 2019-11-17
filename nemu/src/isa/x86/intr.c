#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags.value);
  rtl_push(&cpu.cs);
  rtl_push(&ret_addr);
  s0 = vaddr_read(cpu.idtr.base+NO*8, 4)+(vaddr_read(cpu.idtr.base+NO*8+4, 4)<<16);
  rtl_j(s0);

}

bool isa_query_intr(void) {
  return false;
}
