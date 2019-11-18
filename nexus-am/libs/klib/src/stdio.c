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
  size_t len = strlen(fmt);
  int is_percent = 0;
  int count = 0;
  
  //while ()

  //*
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
