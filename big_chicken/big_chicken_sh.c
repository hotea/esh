/* big_chicken_sh.c
 *
 * 增加控制流程
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include "chicken_sh.h"

#define DFL_PROMPT "> "			/*设置提示符*/

void setup();
void fatal(char *s1, char *s2, int n); 	/*用于显示错误信息*/


int main()
{
  char *cmdline, *prompt, **arglist;
  int result;
  
  prompt = DFL_PROMPT;
  setup();

  while ((cmdline = next_cmd(prompt, stdin)) != NULL) {
    if ((arglist = splitline(cmdline)) != NULL) {
      result = process(arglist);
      freelist(arglist);
    }
    free(cmdline);
  }
  return 0;
}

void setup()
/* 设置信号处理函数
 */
{
  signal(SIGINT, SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
/* 错误处理函数
 */
{
  fprintf(stderr, "Error: %s, %s\n", s1, s2);
  exit(n);
}
 
