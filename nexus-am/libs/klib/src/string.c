#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while (*s) {
    len++;
    s++;
  }
  
  return len;
}

char *strcpy(char* dst,const char* src) {
  assert(dst != NULL && src != NULL);

  char* r = dst;

  do {
    *dst++ = *src++;
  } while (*src != '\0');
  
  return r;
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(dst != NULL && src != NULL);

  char* r = dst;
  size_t off = 0;

  if (strlen(src) < n) { 
    off = n - strlen(src);
    n = strlen(src);
  }

  while (n--){
    *dst++ = *src++;
  }

  while (off--) {
    *dst++ = '\0';
  }

  return r;
}

char* strcat(char* dst, const char* src) {
  assert(dst != NULL && src != NULL);

  char* r = dst;

  while (*dst != '\0') dst++;

  do {
    *dst++ = *src++;
  } while (*src != '\0');
  
  return r;
}

int strcmp(const char* s1, const char* s2) {
  //_putc('s'); _putc('c'); _putc('m'); _putc('p');

  while (*s1 == *s2) {
    if (*s1 == '\0') return 0;
    s1++;
    s2++;
  }

  return *s1 - *s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  while (*s1 == *s2 && n--) {
    if (*s1 == '\0') return 0;
    s1++;
    s2++;
  }

  return *s1 - *s2;
}

void* memset(void* v,int c,size_t n) {
  char* r = (char*)v;
  for (size_t i=0; i<n; i++) {
    *(r+i) = (char) c;
  }

  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  char* dst = (char*)out;
  char* src = (char*)in;
  /*size_t off = 0;

  if (strlen(src) < n) { 
    off = n - strlen(src);
    n = strlen(src);
  }*/

  while (n--){
    *dst++ = *src++;
  }

  /*while (off--) {
    *dst++ = '\0';
  }*/

  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char* s1_c = (char*) s1;
  char* s2_c = (char*) s2;

  while (*s1_c == *s2_c && n--) {
    s1_c++;
    s2_c++;
  }

  if (*s1_c == *s2_c) return 0;

  return *s1_c - *s2_c;
}

#endif
