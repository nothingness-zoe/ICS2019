#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char *out = (char*) buf;
  for (int i=0; i < len; i++) {
    _putc(*(out+i));
  }
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

#define KEYDOWN_MASK 0x8000

size_t events_read(void *buf, size_t offset, size_t len) {
  int keycode = read_key();
  if (keycode != _KEY_NONE) {
    if (keycode & KEYDOWN_MASK) {
      keycode = keycode & ~KEYDOWN_MASK;
      return sprintf(buf, "kd %s\n", keyname[keycode]);
    }
    else return sprintf(buf, "ku %s\n", keyname[keycode]);
  }
  else {
    uint32_t time = uptime();
    return sprintf(buf, "t %d\n", time);
  }
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  // memcpy(buf, &dispinfo[offset], len);
  // return len;
  return sprintf(buf, (void*)(&dispinfo[0]+offset),len);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int width = screen_width();
  // int height = screen_height();
  offset /= 4;
  int x = offset % width;
  int y = offset / width;
  draw_rect((uint32_t*)buf, x, y, len/4, 1);
  return len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  // printf("success\n");
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  //Log("Finish initializing devices...");

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
  // char* out = dispinfo;
  // while(*out) printf("%s", out++);
}
