#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
static int p = 0; // position of buf

uint32_t choose(uint32_t n) {
  return rand()%n;
}

static void gen_num() {
  for (int i=0; i < 32; i++) {
    buf[p] = choose(10);
    p++;
    if (p >= 65536) break;
  }
}

static void gen(char s) {
  buf[p] = s;
  p++;
}

static void gen_rand_op() {
  switch(choose(4)) {
    case 0: buf[p] = '+'; break;
    case 1: buf[p] = '-'; break;
    case 2: buf[p] = '*'; break;
    default: buf[p] = '/'; break;
  }
  p++;
}

static inline void gen_rand_expr() {
  if (p >= 65536) return;
  switch(choose(3)) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }

  int i = choose(2);
  if (i == 1 && p < 65536) {
    buf[p] = ' ';
    p++;
  }

  if (p < 65536) buf[p] = '\0';
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  buf[0] = '\0';
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
