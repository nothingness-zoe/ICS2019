#ifndef __MIPS32_REG_H__
#define __MIPS32_REG_H__

#include "common.h"

#define PC_START (0x80000000u + IMAGE_START)

typedef struct {
  struct {
    rtlreg_t _32;
  } gpr[32];

  vaddr_t pc;

  union {
    struct {
      uint32_t CF: 1;
      uint32_t   : 1;
      uint32_t PF: 1;
      uint32_t   : 1;
      uint32_t AF: 1;
      uint32_t   : 1;
      uint32_t ZF: 1;
      uint32_t SF: 1;
      uint32_t TF: 1;
      uint32_t IF: 1;
      uint32_t DF: 1;
      uint32_t OF: 1;
      uint32_t IOPL: 2;
      uint32_t NT: 1;
      uint32_t   : 1;
      uint32_t RF: 1;
      uint32_t VM: 1;
      uint32_t   : 14;
    };
    uint32_t value;
  }eflags;

} CPU_state;

static inline int check_reg_index(int index) {
  assert(index >= 0 && index < 32);
  return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)

static inline const char* reg_name(int index, int width) {
  extern const char* regsl[];
  assert(index >= 0 && index < 32);
  return regsl[index];
}

#endif
