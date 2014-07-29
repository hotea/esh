
/* big_egg_sh.c
 * 虽然鸡蛋不能生长，但这个shell相对egg_sh实在是没多大长进，就只好让原来的蛋长大些了
 */


#include <stdio.h>
#include <signal.h>
#include <string.h>

#define MAXARGS 20      /* 参数的最大个数 */
#define ARGLEN  100     /* 参数缓冲区长度 */ 

char * makestring(char *buf);
int execute(char *arglist[]);



int main()
{
	char *arglist[MAXARGS+1];	/* 参数数组 */
	int numargs = 0;		/* 参数数组索引 */
	char argbuf[ARGLEN];		/* 存放读入内容的缓冲区 */

	signal(SIGINT, SIG_IGN);
	while( numargs < MAXARGS ) {
		printf("arg[%d]? ", numargs);
		if( fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n' )
			arglist[numargs++] = makestring(argbuf);
		else{
			if( numargs > 0 ){
				arglist[numargs] = NULL;
				execute(arglist);
				numargs = 0;
			}
		}
	}
	return 0;
}

int execute(char *arglist[])
/* 使用fork()和execvp(), 用wait()等待子进程
 */
{
	int pid,exitstatus;	/* 子进程的进程号和退出状态 */

	pid = fork();		/* 创建子进程 */
	switch( pid ){
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist); /* 执行在shell中输入的程序 */
			perror("execvp failed");
			exit(1);
		default:
			while(wait(&exitstatus) != pid)
				;
			printf("child exited with status %d, %d\n",exitstatus>>8, exitstatus&0377);	/* 退出信息 */
	}
}

char *makestring(char * buf)
/*
 * 去掉每个参数最后位置的换行，改成'\0'，即C语言的字符串结束符
 * 并为每个参数分配内存，以便存放它们
 */
{
	char *cp;

	buf[strlen(buf)-1] = '\0';	/* 将'\n'改为'\0' */
	cp = malloc(strlen(buf)+1);
	if( cp == NULL ){
		fprintf(stderr, "no memory\n");		/* 从开始学编程到现在，内存不足这个情况我从来没碰到过=_=! */
		exit(1);
	}
	strcpy(cp, buf);	/* 把参数缓冲区里的内容复制到刚分配的地方 */
	return cp;		/* 返回参数所在位置的指针 */
}




