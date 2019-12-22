#include "rtl/rtl.h"

# define IRQ_TIMER 32

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags.value);
  cpu.eflags.IF = 0;
  rtl_push(&cpu.cs);
  rtl_push(&ret_addr);
  s0 = (vaddr_read(cpu.idtr.base+NO*8, 4)&0x0000ffff)|(vaddr_read(cpu.idtr.base+NO*8+4, 4)&0xffff0000);
  rtl_j(s0);

}

bool isa_query_intr(void) {
  if (cpu.eflags.IF & cpu.INTR) {
    cpu.INTR = false;
    raise_intr(IRQ_TIMER, cpu.pc);
    return true;
  }
  return false;
}
