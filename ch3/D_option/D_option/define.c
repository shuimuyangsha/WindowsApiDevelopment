/* ************************************
*《精通Windows API》 
* 示例代码
* define.c
* 3.1.6  示例：使用/D选项进行条件编译
**************************************/

/* 预处理　*/
/* 头文件　*/
#include <stdio.h>

/* 判断申明 */
#ifdef DEBUG_PRINT
#define MyPrint printf("hello\n")
#else
#define MyPrint
#endif

// main函数
int main()
{
	MyPrint;
	return 0;
}