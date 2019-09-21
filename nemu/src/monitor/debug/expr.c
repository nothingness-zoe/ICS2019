#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ = 1, TK_TEN = 10

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"\\-", '-'},			// minus
  {"\\*", '*'},			// multiply
  {"\\/", '/'},			// divide
  {"\\(", '('},			// left parenthesis
  {"\\)", ')'},			// right parenthesis
  {"==", TK_EQ},        // equal
  {"[0-9]+", TK_TEN},   // dec
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        if (rules[i].token_type == TK_NOTYPE) continue; 

        switch (rules[i].token_type) {
          case '+':{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case '-':{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case '*':{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case '/':{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case '(':{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case ')':{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case TK_TEN:{tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          default: assert(0);
        }
        nr_token++;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p, int q) {
  int left = 1;
  int right = 1;
  if (!(tokens[p].type == '(' && tokens[q].type == ')')) return false;
  p++;
  for (; p < q; p++) {
    if (tokens[p].type == '(') left += 1;
    else if (tokens[p].type == ')') right +=1;
    
    if (right > left+1) assert(0); // bad expression
    else if (right == left+1) return false; 
    /* the leftmost "(" and the rightmost ")" are not matched */
  }
  
  if (left != right) return false;

  return true;
}

uint32_t find_main_op(int p, int q) {
  int op = p;
  int left = 0;
  int right = 0;
  for (int i = p; i < q; i++) {
    if (tokens[i].type == '(') left++;
    else if (tokens[i].type == ')') right++;

    if (left == right) {
      if (tokens[i].type == TK_TEN || tokens[i].type == TK_NOTYPE) continue;
      if (tokens[i].type == '+' || tokens[i].type == '-') op = i;
      if (tokens[i].type == '*' || tokens[i].type == '/') {
        if (tokens[op].type == '+' || tokens[op].type == '-') continue;
        else op = i;
      }
    }
  }
}

uint32_t eval(int p, int q) {
  if (p > q) assert(0);
  else if (p == q) {
    int number = 0;
    sscanf(tokens[p].str, "%d", &number);
    return number;
  }
  else if (check_parentheses(p, q) == true) return eval(p+1, q-1);
  else {
    int op;
    op = find_main_op(p, q);
    uint32_t val1, val2;
    val1 = eval(p, op-1);
    val2 = eval(op+1, q);

    switch(tokens[op].type) {
      case '+': return val1+val2; break;
      case '-': return val1-val2; break;
      case '*': return val1*val2; break;
      case '/': return val1/val2; break;
      default: assert(0);
    }
  }
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  
  eval(0, 31);
  return 0;
}
