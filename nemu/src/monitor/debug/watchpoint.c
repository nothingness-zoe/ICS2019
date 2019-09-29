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

void info_wp() {
  WP *p = head;
  if( p == NULL) 
    print ("No watchpoint.\n");
  else {
    for(; p != NULL; p = p->next)
      printf ("%d %s %#x\n", p->NO,p->expr,p->val1);
  }
}

void set_wp(char *e) {
  WP* p;
  p = new_up();
  printf("Set watchpoint %d\n", p->NO);
  strcpy(p->expr, e);
  bool success = true;
  p->val1 = expr(p->expr, &success);
  if (!success) print("Bad expression.\n");
  else printf("Old value = %#x\n", p->expr);
}

WP* scan_wp() {
  WP* p = head;
  bool success = true;
  if(p == NULL) {
    printf("No watchpoint\n");
    return false;
  }
  else {
    for (; p != NULL; p=p->next) {
      p->val2 = expr(p->expr, &success);
      if (!success)
        printf ("Fail to eval val2 in watchpoint %d", p->NO);
      else {
        if (p->val1 != p->val2) {
          printf ("Trigger watchpoint %d\n", p->NO);
          p->val1 = p->val2;
          return p;
        }
      }
    }
  }
  return NULL;
}