#include"LexicalAnalysis.h"
#include<stdio.h>
/**
	@brief 单词哈希表
*/
TkWord * tk_hashtable[1024]; 

/**
	@brief 单词表
*/
DynArray tktable; 

/**
	@brief 单词字符串
*/
DynString tkstr;

/**
	@brief 单词源码字符串
*/
DynString sourcestr;

/**
	@brief 单词值（单词为整形常量）
*/
int tkvalue;

/**
	@brief 当前取到的源码字符
*/
char ch;

/**
	@brief 单词编码
*/
int token;

/**
	@brief 行号
*/
int line_num;
char* filename;

FILE* fin;


int main(int argc,char **argv){

	printf("hhhhhhh");
	fin = fopen(argv[1],"rb");
	filename = argv[1];
	if(!fin){
		printf("不能打开源文件！\n");
	}

	init();
	getch();
	do{
		get_token();
		color_token(LEX_NORMAL);
	}while(token != TK_EOF);

	printf("\n代码行数：%d\n",line_num);
	cleanup();
	fclose(fin);
	printf("%s词法分析成功！",argv[1]);
	return 0;

}