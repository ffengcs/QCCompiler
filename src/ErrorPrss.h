#ifndef ERROR_PROCESSING_H
#define ERROR_PROCESSING_H
#include<stdarg.h>
/**
	@brief 错误级别
*/
enum e_ErrorLevel
{
	LEVEL_WARNING,
	LEVEL_ERROR
};

/**
	@brief 工作阶段
*/
enum e_WorkStage
{
	STAGE_COMPILE,
	STAGE_LINK
};

/**
	@brief 异常处理
	@param stage 编译阶段还是链接阶段
	@param level 错误级别
	@param fmt 参数输出的格式
	@param ap 可变参数列表
*/
void handle_exception(int stage,int level, char* fmt,va_list ap);

/**
	@brief 编译警告处理
	@param fmt 参数输出格式
	@param ap 可变参数列表
*/
void warning(char* fmt,...);

/**
	@brief 编译器致命错误处理
	@param fmt 参数输出格式
	@param ap 可变参数列表
*/
void error(char* fmt,...);

/**
	@brief 提示错误，此处缺少某个语法成分
	@param msg:需要什么语法成分
*/
void expect(char *msg);

/**
	@brief 跳过单词c，取下一个单词，如果当前单词不是c,提示错误
	@param c 要跳过的单词
*/
void skip(int c);

/**
	@brief 取得单词v所代表的源码字符串
	@param v 单词编号
*/
char * get_tkstr(int v);

/**
	@brief 链接错误处理
	@param fmt 参数输出格式
	@param ap 可变参数列表
*/
void link_error(char* fmt,...);



#endif