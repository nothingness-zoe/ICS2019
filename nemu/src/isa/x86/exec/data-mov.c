#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);
  //printf("cpu.esp: %#x\n", cpu.esp);
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  operand_write(id_dest, &id_dest->val);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  //printf("pusha cpu.esp: %#x\n", cpu.esp);

  s0 = cpu.esp;
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&s0);
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);
  //printf("pusha cpu.esp: %#x\n", cpu.esp);

  print_asm("pusha");
}

make_EHelper(popa) {
  //TODO();
  printf("popa cpu.esp: %#x\n", cpu.esp);
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);
  printf("popa cpu.esp: %#x\n", cpu.esp);
  
  print_asm("popa");
}

make_EHelper(leave) {
  rtl_mv(&cpu.esp, &cpu.ebp);
  rtl_pop(&cpu.ebp);
  //printf("cpu.esp: %#x\n", cpu.esp);

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    //TODO();
    rtl_lr(&s0, R_AX, 2);
    if((int32_t)(int16_t)(uint16_t)s0 < 0) {
      s1 = 0xffff;
      rtl_sr(R_DX, &s1, 2);
    }
    else {
      s1 = 0;
      rtl_sr(R_DX, &s1, 2);
    }
  }
  else {
    //TODO();
    rtl_lr(&s0, R_EAX, 4);
    if((int32_t)s0 < 0) {
      s1 = 0xffffffff;
      rtl_sr(R_EDX, &s1, 4);  
    }
    else {
      s1 = 0;
      rtl_sr(R_EDX, &s1, 4);
    }
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    //TODO();
    rtl_lr(&s0, R_AL, 1);
    rtl_sext(&s0, &s0, 1);
    rtl_sr(R_AX, &s0, 2);
  }
  else {
    //TODO();
    rtl_lr(&s0, R_AX, 2);
    rtl_sext(&s0, &s0, 2);
    rtl_sr(R_EAX, &s0, 4);
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  //printf("cpu.esp: %#x\n", cpu.esp);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
