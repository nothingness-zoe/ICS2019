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

typedef long ssize_t;
typedef size_t off_t;

_Context* do_syscall(_Context *c);

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

size_t fs_filesz(int fd);
int fs_open (const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void * buf, size_t len);
ssize_t fs_write(int fd, const void * buf, size_t len);
off_t fs_lseek (int fd, off_t offset, int whence);
ssize_t fs_close(int fd);

size_t serial_write(const void *buf, size_t offset, size_t len);

#endif
