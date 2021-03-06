#include "rtl/rtl.h"

/* Condition Code */

void rtl_setcc(rtlreg_t* dest, uint8_t subcode) {
  bool invert = subcode & 0x1;
  enum {
    CC_O, CC_NO, CC_B,  CC_NB,
    CC_E, CC_NE, CC_BE, CC_NBE,
    CC_S, CC_NS, CC_P,  CC_NP,
    CC_L, CC_NL, CC_LE, CC_NLE
  };

  //printf("CF: %d  OF: %d  ZF: %d SF: %d\n", cpu.eflags.CF, cpu.eflags.OF, cpu.eflags.ZF, cpu.eflags.SF);
  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O: // 0x90
      rtl_get_OF(dest);
      break;
    case CC_B: // 0x92
      rtl_get_CF(dest);
      break;
    case CC_E: // 0x94
      rtl_get_ZF(dest);
      break;
    case CC_BE: { // 0x96
      rtl_get_CF(&t0);
      rtl_get_ZF(&t1);
      rtl_or(dest, &t0, &t1);
    } break;
    case CC_S: // 0x98
      rtl_get_SF(dest);
      break;
    case CC_L: { // 0x9c
      rtl_get_SF(&t0);
      rtl_get_OF(&t1);
      rtl_xor(dest, &t0, &t1);
    } break;
    case CC_LE: { // 0x9e
      rtl_get_SF(&t0);
      rtl_get_OF(&t1);
      rtl_xor(&t0, &t0, &t1);
      rtl_get_ZF(&t1);
      rtl_or(dest, &t0, &t1);
    } break;
    default: panic("should not reach here");
    case CC_P: panic("n86 does not have PF");
  }

  if (invert) {
    rtl_xori(dest, dest, 0x1); // 其他情况，如 CC_NO
  }
  assert(*dest == 0 || *dest == 1);
}
