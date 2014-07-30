/* 执行命令的部分
 */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

int execute(char *argv[])
{
  int pid;
  int child_info = -1;

  if (argv[0] == NULL)
    return 0;

  if ((pid = fork()) == -1)
    perror("fork");
  else if (pid == 0) {			/* 在子进程中恢复信号的默认处理 */
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    execvp(argv[0],argv);
    perror("cannot execute command");
    exit(1);
  }else {
    if (wait(&child_info) == -1)
      perror("wait");
  }
  return child_info;
}
