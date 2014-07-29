

/* egg_sh.c
 *   你认为是先有蛋呢还是鸡呢，这个连鸡和蛋自己都不知道的问题困扰了愚蠢的人类很长时间，姑且认为先有蛋吧，此残疾shell被命名为egg_sh
 *   by the way, 使用大写字母开头分隔程序名是很丑陋的，比如EggSh, 真正的程序员用"_"分隔程序名
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
{
	execvp(arglist[0], arglist);	/* 此处即开始执行程序中的程序, arglist[0]为新程序的名称，arglist为参数列表 */
	perror("execvp failed");
	exit(1);
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




