/*splitline.c
 * 为chicken_sh读取并解析命令
 *char *next_cmd(char *prompt, FILE *fp)  取下一条指令
 *char **splitline(char *str);            解析字符串
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "chicken_sh.h"

char *next_cmd(char *prompt, FILE * fp)
{
    char *buf;
    int bufspace = 0;
    int pos=0;		/* 当前位置 */
    int c;		

    printf("%s", prompt);
    while ((c = getc(fp)) != EOF) {
	/*若需要空间*/
	if (pos + 1 >= bufspace) {
	    if (bufspace == 0)	
		buf = emalloc(BUFSIZ);
	    else	
		buf = erealloc(buf, bufspace + BUFSIZ);	/* 扩大分配的内存 */
	    bufspace += BUFSIZ;	
	}

	/* 命令结束 */
	if (c == '\n')
	    break;

	/* 如果不结束，则添加进缓冲区 */
	buf[pos++] = c;
    }
    if (c == EOF && pos == 0)	
	return NULL;
    buf[pos] = '\0';
    return buf;
}


#define is_delim(x) ((x) == ' ' || (x) == '\t')    /*参数分隔符是空格或tab*/

char *newstr(char *s, int l);
char **splitline(char *line)
{
    char **args;	/*要返回的参数数组*/
    int spots = 0;	/*参数指针的容量*/	
    int bufspace = 0;	/*缓冲空间*/	
    int argnum = 0;	/*参数计数*/	
    char *cp = line;		
    char *start;
    int len;

    if (line == NULL)		/* 什么输入也没有 */
	return NULL;

    args = emalloc(BUFSIZ);	/* 分配参数数组 */
    bufspace = BUFSIZ;
    spots = BUFSIZ / sizeof(char *);

    while (*cp != '\0') {
	while (is_delim(*cp))	
	    cp++;
	if (*cp == "\0")
	    break;
	/* 确保参数数组的空间 */
	if (argnum + 1 >= spots) {
	    args = erealloc(args, bufspace + BUFSIZ);
	    bufspace += BUFSIZ;
	    spots += (BUFSIZ / sizeof(char *));
	}

	/* 标记开始的地方，查找以\0 结束的位置 */
	start = cp;
	len = 1;
	while (*++cp != '\0' && !(is_delim(*cp)))
	    len++;
	args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;
    return args;
}

/*
 * 构造字符串，以'\0' 结尾*/
char *newstr(char *s, int l)
{
    char *rv = emalloc(l + 1);

    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char **list)
	/*参数用完后，释放空间*/
{
    char **cp = list;
    while (*cp)
	free(*cp++);
    free(list);
}

void *emalloc(size_t n)
{
    void *rv;
    if ((rv = malloc(n)) == NULL)
	fatal("out of memory", "", 1);
    return rv;
}

void *erealloc(void *p, size_t n)
{
    void *rv;
    if ((rv = realloc(p, n)) == NULL)
	fatal("realloc() failed", "", 1);
    return rv;
}
