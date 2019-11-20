#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if (  ref_r->eax != cpu.eax ||ref_r->ecx != cpu.ecx || ref_r->edx != cpu.edx 
  || ref_r->ebx != cpu.ebx || ref_r->esp != cpu.esp || ref_r->ebp != cpu.ebp
  || ref_r->esi != cpu.esi || ref_r->edi != cpu.edi ) {
    if (ref_r->eax != cpu.eax)
      printf("diff:\ncpu.eax=%#x,ref.eax=%#x\n", cpu.eax,ref_r->eax);
    if (ref_r->ecx != cpu.ecx)
      printf("diff:\ncpu.ecx=%#x,ref.ecx=%#x\n", cpu.ecx,ref_r->ecx);
    if (ref_r->edx != cpu.edx)
      printf("diff:\ncpu.edx=%#x,ref.edx=%#x\n", cpu.edx,ref_r->edx);
    if (ref_r->ebx != cpu.ebx)
      printf("diff:\ncpu.ebx=%#x,ref.ebx=%#x\n", cpu.ebx,ref_r->ebx);
    if (ref_r->esp != cpu.esp)
      printf("diff:\ncpu.esp=%#x,ref.esp=%#x\n", cpu.esp,ref_r->esp);
    if (ref_r->ebp != cpu.ebp)
      printf("diff:\ncpu.ebp=%#x,ref.ebp=%#x\n", cpu.ebp,ref_r->ebp);
    if (ref_r->esi != cpu.esi)
      printf("diff:\ncpu.esi=%#x,ref.esi=%#x\n", cpu.esi,ref_r->esi);
    if (ref_r->edi != cpu.edi)
      printf("diff:\ncpu.edi=%#x,ref.edi=%#x\n", cpu.edi,ref_r->eax);
    /*if (ref_r->pc != pc)
      printf("diff:\npc=%#x,ref.pc=%#x\n", pc,ref_r->pc);*/
    printf("position: %#x\n", cpu.pc);
    return false;
  }
    
  else return true;
}

void isa_difftest_attach(void) {
}
