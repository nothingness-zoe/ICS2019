#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

void difftest_detach();

void difftest_attach();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_detach(char *args);

static int cmd_attach(char *args);


static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Suspend the execution of the program after stepping through N instructions. When N is not given, the default is 1", cmd_si },
  { "info", "Print register status with r. Print watchpoint information with w", cmd_info},
  { "x", "Output N 4-byte in hexadecimal format from the start memory address", cmd_x},
  { "p", "Evaluate the expression", cmd_p},
  { "w", "Set watchpoint", cmd_w},
  { "d", "Delete watchpoint", cmd_d},
  { "detach", "Exit DiffTest mode", cmd_detach},
  { "attach", "Enter DiffTest mode", cmd_attach},

  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args) {
	char *arg = strtok(NULL, " ");
	int step = 0;
	if (arg == NULL) cpu_exec(1);
	else {
		sscanf(arg, "%d", &step);
		if (step <= 0) cpu_exec(-1);
		else cpu_exec(step);
	}
	return 0;
}

static int cmd_info(char *args) {
	char *arg = strtok(NULL, " ");	
	if (strcmp(arg, "r") == 0) isa_reg_display();
  else if (strcmp(arg, "w") == 0) info_wp();
	return 0;
}

static int cmd_x(char *args) {
	vaddr_t start, next, temp;
	int time, i;
	char *arg = strtok(NULL, " ");
	char *arg_1 = strtok(NULL, " ");
	sscanf(arg, "%d", &time);
	sscanf(arg_1, "%x", &start);
	printf("%#x:", start);
	next = start;
	for (i = 0; i < time; i++){
		temp = paddr_read(next, 4);
		printf("  %#x", temp);
		next += 4;
	}
	return 0;
}

static int cmd_p(char *args) {
  char *arg = strtok(NULL, "@");
  bool success = true;
  int result = expr(arg, &success);
  if (success) printf("%#x\n", result);
  else printf("Bad expression.\n");
  return 0;
}

static int cmd_w(char *args) {
  char *arg = strtok(NULL, " ");
  set_wp(arg);
  return 0;
}
static int cmd_d(char *args) {
  char *arg = strtok(NULL, " ");
  int n;
  sscanf(arg, "%d", &n);
  free_wp(n);
  return 0;
}

static int cmd_detach(char *args) {
  difftest_detach();
  printf("Exit DiffTest\n");
  return 0;
}

static int cmd_attach(char *args) {
  difftest_attach();
  printf("Enter DiffTest\n");
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
