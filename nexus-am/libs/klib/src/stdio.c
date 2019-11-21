#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char out[1024];
  va_list ap;
  va_start(ap, fmt);
  int n = vsprintf(out, fmt, ap);
  va_end(ap);
  //while(*out) _putc(*out++);
  int len = strlen(out);
  for(int i=0; i < len; i++) {
    _putc(out[i]);
  }
  return n;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  //size_t len = strlen(fmt);
  //int is_percent = 0;
  int count = 0;
  int have_head = 0;
  
  while (*fmt) {
    have_head = 0;
    if(*fmt == '%') {
      char op = *++fmt;
      while ('0' <= op && op <= '9') op = *++fmt;
      switch (op) {
        case 's': {
          char *str = va_arg(ap, char*);
          while (*str) {
            *out++ = *str++;
            count++;
          }
        } break;
        case 'd': {
          int num = va_arg(ap, int);
          char buf[128];
          int i=0;
          if (num < 0) {
            *out++ = '-';
            num = -num;
          }
          if (num == 0) *out++ = '0';
          else {
            while (num) {
            buf[++i] = num%10 +'0';
            num /= 10;
            }
          }
          while (num) {
            buf[++i] = num%10 +'0';
            num /= 10;
          }
          while(i) {
            *out++ = buf[i--];
            count++;
          }
        } break;
        case 'p': have_head=1;
        case 'x': {
          int num = va_arg(ap, int);
          char buf[128];
          int i = 0;
          int mod = 0;
          // _putc('0'); _putc('\n');
          do {
            mod = num%16;
            if (mod <= 9) buf[i] = mod+'0';
            else buf[i] = mod+('A'-10);
            num /= 16;
            i++;
          } while(num);
          buf[i] = '\0';
          // _putc('1'); _putc('\n');
          if (have_head) {
            *out++ = '0';
            count++;
            *out++ = 'x';
            count++;
          }
          while(i) {
            *out++ = buf[i-1];
            i--;
            count++;
          } break;
        }
        default: _putc(op); _putc('\n'); break;
      }
      fmt++;
    }
    else {
      *out++ = *fmt++;
      count++;
    }
  }

  /*
  for (size_t i=0; i < len; i++) {
    // TODO: my_todo case %b %c %f %o %x 
    switch(fmt[i]) {
      case '%': {
        if (is_percent) {
          *out++ = '%';
          is_percent = 0;
        }
        else is_percent = 1;
      } break;
      case 'd': {
        if (is_percent) {
          int num = va_arg(ap, int);
          char buf[128];
          int i=0;
          while (num) {
            buf[i++] = num%10 +'0';
            num /= 10;
          }
          while(i--) *out++ = buf[i];
          count++;
          is_percent = 0;
        }
        else *out++ = 'd';
      } break;
      case 's': {
        if (is_percent) {
          char *str = va_arg(ap, char*);
          while (*str) *out++ = *str++;
          count++;
          is_percent = 0;
        }
        else *out++ = 's';
      } break;
      default: *out++ = fmt[i]; break;
    }

  }
  //*/
  *out = '\0';
  return count;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int n = vsprintf(out, fmt, ap);
  va_end(ap);
  return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  //TODO: my_todo
  _putc('!');
  _putc('U');
  _putc('F');
  return 0;
}

#endif
