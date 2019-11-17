#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  uintptr_t esi, ebx, eax, eip, edx, eflags, ecx, cs, esp, edi, ebp;
  struct _AddressSpace *as;
  int irq;
};

// #define GPR1 eax
// #define GPR2 eip
// #define GPR3 eip
// #define GPR4 eip
// #define GPRx eip

#define GPR1 eax
#define GPR2 ebx
#define GPR3 ecx
#define GPR4 edx
#define GPRx eax

#endif
