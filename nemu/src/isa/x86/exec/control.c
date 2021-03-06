#include "cpu/exec.h"
#include "cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(decinfo.jmp_pc);

  print_asm("jmp %x", decinfo.jmp_pc);
}

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  //printf("jcc\n");
  uint32_t cc = decinfo.opcode & 0xf;
  rtl_setcc(&s0, cc);
  //printf("after setcc s0=%d\n", s0);
  rtl_li(&s1, 0);
  rtl_jrelop(RELOP_NE, &s0, &s1, decinfo.jmp_pc);

  //printf("jcc success\n");

  print_asm("j%s %x", get_cc_name(cc), decinfo.jmp_pc);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  //printf("start call\n");
  rtl_push(&decinfo.seq_pc);
  rtl_j(decinfo.jmp_pc);
  //printf("calling:0x%-10x\n", decinfo.jmp_pc);

  print_asm("call %x", decinfo.jmp_pc);
}

make_EHelper(ret) {
  rtl_pop(&decinfo.jmp_pc);
  rtl_j(decinfo.jmp_pc);
  //printf("cpu.esp: %#x\n", cpu.esp);

  print_asm("ret");
}

make_EHelper(ret_imm) {
  rtl_addi(&cpu.esp, &cpu.esp, 2);

  print_asm("ret %s", id_dest->str);
}

make_EHelper(call_rm) {
  rtl_push(&decinfo.seq_pc);
  rtl_jr(&id_dest->val);

  print_asm("call *%s", id_dest->str);
}
