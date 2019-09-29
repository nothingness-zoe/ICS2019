#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ = 1, TK_UEQ = 0,
  TK_DEC = 10, TK_HEX = 16, TK_REG = 255,
  TK_POINT = 9

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
  {"\\-", '-'},			    // minus
  {"\\*", '*'},			    // multiply
  {"\\/", '/'},			    // divide
  {"\\(", '('},			    // left parenthesis
  {"\\)", ')'},			    // right parenthesis
  {"\\$[a-ehilpx]{2,3}", TK_REG},   // register
  {"==", TK_EQ},        // equal
  {"!=", TK_UEQ},       // unequal
  {"[0-9]+", TK_DEC},   // dec
  {"0x[0-9a-f]+", TK_HEX},   // hex
  {"&&", '&'}           // and
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
          case TK_DEC:{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case TK_HEX:{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case TK_POINT:{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case TK_EQ:{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case TK_UEQ:{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case TK_REG:{
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
          } break;
          case '&':{
            tokens[nr_token].type = rules[i].token_type;
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
      if (tokens[i].type == TK_DEC || tokens[i].type == TK_NOTYPE || \
      tokens[i].type == TK_HEX || tokens[i].type == TK_POINT) continue;
      if (tokens[i].type == '&' || tokens[i].type == TK_UEQ || tokens[i].type == TK_EQ)
        op = i;
      if (tokens[op].type != '&' && tokens[op].type != TK_EQ && tokens[op].type != TK_UEQ) {
        if (tokens[i].type == '+' || tokens[i].type == '-') {
          if (tokens[i].type == '-' && (i == 0 || tokens[i-1].type == '+' || tokens[i-1].type == '-' || tokens[i-1].type == '*' || tokens[i-1].type == '/')) continue;
          else op = i;  // 判断减号是否为负数标志
        }
        if (tokens[i].type == '*' || tokens[i].type == '/') {
          if (tokens[op].type == '+' || tokens[op].type == '-') continue;
          else op = i;
        }
      }
    }
  }
  return op;
}

uint32_t eval(int p, int q) {
  if (p > q) assert(0);
  else if (p == q) {
    int number = 0;
    if (tokens[p].type == TK_DEC) sscanf(tokens[p].str, "%d", &number);
    else if (tokens[p].type == TK_HEX) sscanf(tokens[p].str, "%x", &number);
    else if (tokens[p].type == TK_REG) {
      for (int i=0; i < 4; i++) tokens[p].str[i] = tokens[p].str[i+1]; // 去$
    }
    return number;
  }
  else if (check_parentheses(p, q) == true) return eval(p+1, q-1);
  else {
    int op;
    op = find_main_op(p, q);

    // 判断是否为指针或者负数
    if (op == p && tokens[op].type == TK_POINT) return paddr_read(eval(p+1, q), 4);
    if (op == p && tokens[op].type == '-') return -eval(p+1, q);
    
    uint32_t val1, val2;
    val1 = eval(p, op-1);
    val2 = eval(op+1, q);

    
    switch(tokens[op].type) {
      case '+': return val1+val2; break;
      case '-': return val1-val2; break;
      case '*': return val1*val2; break;
      case '/': return val1/val2; break;
      case '&': return val1&val2; break;
      case TK_UEQ: return val1!=val2; break;
      case TK_EQ: return val1==val2; break;
      default: assert(0);
    }
  }
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  //return 0;}

  /* TODO: Insert codes to evaluate the expression. */
  
  for(int i = 0; i < nr_token; i++) {
    if(tokens[i].type == '*' && (i == 0 || tokens[i-1].type == '+' || \
    tokens[i-1].type == '-' || tokens[i-1].type == '*' || tokens[i-1].type == '/')) {
      tokens[i].type = TK_POINT;
    }
  }  // 判断是否为指针类型

  return eval(0, nr_token-1);
}