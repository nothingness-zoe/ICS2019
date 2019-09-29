#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */


WP* new_up() {
  WP *p = free_;
  if (p == NULL) assert(0);
  free_ = free_->next;
  p->next = head;
  head = p;
  return p;
}

void free_wp(int n) { // 要求删除第n个节点，故将输入改成n
  if (head == NULL) return;
  WP *p = head;
  if (head->NO == n) {
    head = head->next;
    p->next = free_;
    free_ = p;
  }
  else {
    WP *p_pre = p;
    for (int i=0; i < 32; i++) {
      if (p->NO == n) {
        p_pre->next = p->next;
        p->next = free_;
        free_ = p;
        return;
      }
      p_pre = p;
      p = p->next;
    }
  }
}