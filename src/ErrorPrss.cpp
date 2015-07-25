#include"ErrorPrss.h"
#include"LexicalAnalysis.h"
#include<stdio.h>
#include<stdlib.h>
#include"GlobalVar.h"

/**
	@brief 异常处理
	@param stage 编译阶段还是链接阶段
	@param level 错误级别
	@param fmt 参数输出的格式
	@param ap 可变参数列表
*/
void handle_exception(int stage,int level, char* fmt,va_list ap){
	char buf[1024];
	vsprintf(buf,fmt,ap);
	if(stage == STAGE_COMPILE){
	
		if(level == LEVEL_WARNING){
			printf("%s(第%d行)：编译警告：%s!\n",filename,line_num,buf);
		}else{
			printf("%s(第%d行)：编译错误：%s!\n",filename,line_num,buf);
			exit(-1);
		}
	}else{
	
		printf("链接错误：%s!\n",buf);
		exit(-1);
	}
}

/**
	@brief 编译警告处理
	@param fmt 参数输出格式
	@param ap 可变参数列表
*/
void warning(char* fmt,...){

	va_list ap;
	va_start(ap,fmt);
	handle_exception(STAGE_COMPILE,LEVEL_WARNING,fmt,ap);
	va_end(ap);
}

/**
	@brief 编译器致命错误处理
	@param fmt 参数输出格式
	@param ap 可变参数列表
*/
void error(char* fmt,...){
	va_list ap;
	va_start(ap,fmt);
	handle_exception(STAGE_COMPILE,LEVEL_ERROR,fmt,ap);
	va_end(ap);
}

/**
	@brief 提示错误，此处缺少某个语法成分
	@param msg:需要什么语法成分
*/
void expect(char *msg){

	error("缺少%s",msg);
}

/**
	@brief 跳过单词c，取下一个单词，如果当前单词不是c,提示错误
	@param c 要跳过的单词
*/
void skip(int c){

	if(token!=c){
		error("缺少'%s'",get_tkstr(c));
	}
}

/**
	@brief 取得单词v所代表的源码字符串
	@param v 单词编号
*/
char * get_tkstr(int v){
	if(v>tktable.count){
		return NULL;
	}else if(v>TK_CINT && v<TK_CSTR){
		return sourcestr.data;
	}else{
		return ((TkWord*)tktable.data[v])->spelling;
	}
}

/**
	@brief 链接错误处理
	@param fmt 参数输出格式
	@param ap 可变参数列表
*/
void link_error(char* fmt,...){

	va_list ap;
	va_start(ap,fmt);
	handle_exception(STAGE_LINK,LEVEL_ERROR,fmt,ap);
	va_end(ap);
}