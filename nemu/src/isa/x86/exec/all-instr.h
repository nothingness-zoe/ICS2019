#include "cpu/exec.h"

make_EHelper(mov);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

// dummy.c
make_EHelper(call);
make_EHelper(push);
make_EHelper(sub);
make_EHelper(xor);
make_EHelper(pop);
make_EHelper(ret);
make_EHelper(ret_imm);

// add.c
make_EHelper(lea);
make_EHelper(and);
make_EHelper(add);
make_EHelper(cmp);
make_EHelper(setcc);
make_EHelper(movzx);
make_EHelper(test);
make_EHelper(jcc);
make_EHelper(leave);
make_EHelper(inc);
make_EHelper(nop);

// add-longlong.c
make_EHelper(adc);
make_EHelper(or);

// quick-sort.c
make_EHelper(jmp);
make_EHelper(dec);

// min3.c
make_EHelper(shl);
make_EHelper(shr);

// recursion.c
make_EHelper(jmp_rm);
make_EHelper(call_rm);
make_EHelper(not);
make_EHelper(neg);
make_EHelper(imul1);
make_EHelper(sar);

// matrix-mul.c
make_EHelper(imul2);
make_EHelper(idiv);

// load-store.c
make_EHelper(movsx);

make_EHelper(pusha);
make_EHelper(popa);
make_EHelper(cwtl);
make_EHelper(cltd);
make_EHelper(sbb);
make_EHelper(in);
make_EHelper(out);
make_EHelper(mul);
make_EHelper(div);
make_EHelper(imul3);
make_EHelper(rol);
make_EHelper(lidt);

/*// control.c
make_EHelper(jmp);
make_EHelper(jmp_rm);

make_EHelper(ret_imm);
make_EHelper(call_rm);*/