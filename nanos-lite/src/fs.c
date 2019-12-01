#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

size_t fs_filesz(int fd) {
  return file_table[fd].size;
}

int fs_open (const char *pathname, int flags, int mode){
  for (int i=0; i < NR_FILES; i++) {
    if (strcmp(file_table[i].name, pathname) == 0) {
      printf("%s\n", pathname);
      return i;
    }
  }
  printf("%s not found\n", pathname);
  assert(0);
  return 0;
}

size_t fs_read(int fd, void * buf, size_t len) {
  //if (file_table[fd].read == NULL) {
    size_t aval_size = fs_filesz(fd) - file_table[fd].open_offset;
    if (aval_size < len) len = aval_size;
    ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
    return len;
  //}
  //else return -1;
}

size_t fs_write(int fd, const void * buf, size_t len) {
  //if (file_table[fd].write == NULL) { 
    size_t aval_size = fs_filesz(fd) - file_table[fd].open_offset;
    if (aval_size < len) len = aval_size;
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
    return len;
  //}
  //else return -1;
}

size_t fs_lseek (int fd, size_t offset, int whence) {
  size_t size = fs_filesz(fd);
  switch (whence) {
    case SEEK_SET: {
      if (offset > size || offset < 0) return -1;
      file_table[fd].open_offset = offset;
      return offset;
    } break;
    case SEEK_CUR: {
      offset += file_table[fd].open_offset;
      if (offset > size || offset < 0) return -1;
      file_table[fd].open_offset = offset;
      return offset;
    } break;
    case SEEK_END: {
      offset += size;
      if (offset > size || offset < 0) return -1;
      file_table[fd].open_offset = offset;
      return offset;
    } break;
    default: return -1; break;
  }
}

int fs_close(int fd) { 
  return 0;
}