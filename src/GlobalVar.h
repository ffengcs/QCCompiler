#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include"LexicalAnalysis.h"

/**
	@brief 单词哈希表
*/
extern TkWord * tk_hashtable[1024]; 

/**
	@brief 单词表
*/
extern DynArray tktable; 

/**
	@brief 单词字符串
*/
extern DynString tkstr;

/**
	@brief 单词源码字符串
*/
extern DynString sourcestr;

/**
	@brief 单词值（单词为整形常量）
*/
extern int tkvalue;

/**
	@brief 当前取到的源码字符
*/
extern char ch;

/**
	@brief 单词编码
*/
extern int token;

/**
	@brief 行号
*/
extern int line_num;
extern FILE* fin;
extern char* filename;

#endif
