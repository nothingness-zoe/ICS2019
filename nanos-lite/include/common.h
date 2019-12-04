#ifndef __COMMON_H__
#define __COMMON_H__

/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE
//#define HAS_VME

#include <am.h>
#include <klib.h>
#include "debug.h"

typedef char bool;
#define true 1
#define false 0

_Context* do_syscall(_Context *c);

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

size_t fs_filesz(int fd);
int fs_open (const char *pathname, int flags, int mode);
size_t fs_read(int fd, void * buf, size_t len);
size_t fs_write(int fd, const void * buf, size_t len);
size_t fs_lseek (int fd, size_t offset, int whence);
int fs_close(int fd);

int screen_width();
int screen_height();
void draw_rect(uint32_t *pixels, int x, int y, int w, int h);
void draw_sync();

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t fbsync_write(const void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);

#endif
