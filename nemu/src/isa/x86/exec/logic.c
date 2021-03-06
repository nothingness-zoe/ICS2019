#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  rtl_and(&s0, &id_dest->val, &id_src->val);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);

  rtl_update_ZFSF(&s0, id_dest->width);
  

  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);
  
  print_asm_template2(and);
}

make_EHelper(xor) {
  //printf("xor id_dest->val= %#x, id_src_val=%#x\n", id_dest->val, id_src->val);
  //printf("id_dest->reg:%#x\n", id_dest->reg);
  //printf("id_dest->addr:%#x\n", id_dest->addr);
  //printf("cpu.eax: %#x\n", cpu.eax);

  
  rtl_xor(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  
  //printf("s0: %#x\n", s0);
  //printf("id_dest->reg:%#x\n", id_dest->reg);
  //printf("id_dest->addr:%#x\n", id_dest->addr);
  //printf("cpu.eax: %#x\n", cpu.eax);
  
  rtl_update_ZFSF(&s0, id_dest->width);
  //printf("xor id_dest->val= %#x, id_src_val=%#x\n", id_dest->val, id_src->val);
  //printf("s0:%#x\n", s0);
  //printf("cpu.eax: %#x\n", cpu.eax);
  
  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  //printf("cpu.eax: %#x\n", cpu.eax);



  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(or);
}

make_EHelper(sar) {
  // unnecessary to update CF and OF in NEMU
  rtl_sar(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(shl);
}

make_EHelper(shr) {
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  rtl_update_ZFSF(&s0, id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(rol) {
  for(int i = 0; i < id_src->val; i++) {
    rtl_shri(&s0, &id_dest->val, id_dest->width*8-1);
    rtl_shli(&s0, &id_dest->val, 1);
    rtl_xori(&s1, &id_dest->val, s0);
  }
  rtl_set_CF(&s0);
  operand_write(id_dest, &s1);

  print_asm_template2(rol);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  rtl_not(&s0, &id_dest->val);
  operand_write(id_dest, &s0);

  print_asm_template1(not);
}
