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

/*// control.c
make_EHelper(jmp);
make_EHelper(jmp_rm);

make_EHelper(ret_imm);
make_EHelper(call_rm);*/