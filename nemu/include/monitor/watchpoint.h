#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[32];  // 储存表达式
  int val_1;  // 要求比较待监视表达式有没有变化，故需两个int变量记录
  int val_2;  // 表达式前后的值


  /* TODO: Add more members if necessary */


} WP;


WP* new_up();
void free_wp(int n);
void info_wp();
void set_wp(char *e);
WP* scan_wp();

#endif
