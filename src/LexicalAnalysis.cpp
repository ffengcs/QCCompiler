#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"LexicalAnalysis.h"
#include"ErrorPrss.h"
#include"GlobalVar.h"
#include<Windows.h>



/**
    @brief 初始化动态字符串存储容量
    @param pstr
    @param initsize
*/
void dynstring_init(DynString* pstr, int initsize){

    if(pstr != NULL){
		pstr->data = (char*)malloc(sizeof(char) * initsize);
        pstr->count = 0;
        pstr->capacity = initsize;
    }
}

/**
    @brief 释放动态字符串使用的内存空间
    @param pstr
*/
void dynstring_free(DynString * pstr){
    if(pstr!=NULL){
        if(pstr->data)
            free(pstr->data);
        pstr->count = 0;
        pstr->capacity = 0;
    }
}

/**
    @brief 重置动态字符串，先释放，重新初始化
    @param pstr
*/
void dynstring_reset(DynString* pstr){
    dynstring_free(pstr);
    dynstring_init(pstr,8);
}

/**
    @brief 重新分配字符串容量
    @param pstr
    @param new_size
*/
void dynstring_realloc(DynString* pstr,int new_size){
    int capacity;
    char* data;
    capacity = pstr->capacity;
    while(capacity<new_size){
        capacity = capacity * 2;
    }
    data = (char*)realloc(pstr->data,capacity);
    if(!data)
		error("内存分配失败");
    pstr->capacity = capacity;
    pstr->data = data;
}

/**
    @brief 追加单个字符到动态字符串对象
    @param pstr
    @param ch
*/
void dynstring_chcat(DynString* pstr,char ch){
    int count;
    count = pstr->count + 1;
    if(count > pstr->capacity){
        dynstring_realloc(pstr,count);
    }
    ((char*)pstr->data)[count-1] = ch;
    pstr->count = count;
}


/**
    @brief 重新分配动态数组容量
    @param parr
    @param new_size
*/
void dynarray_realloc(DynArray* parr,int new_size){
    int capacity;
    void **data;
    capacity = parr->capacity;
    while(capacity<new_size){
        capacity *= 2;
    }
    data = (void**)realloc(parr->data,capacity);
    if(!data){
		error("内存分配失败");
    }
    parr->capacity = capacity;
    parr->data = data;
}

/**
    @brief 追加动态数组元素
    @param parr
    @param data
*/
void dynarray_add(DynArray* parr, void* data){
    int count;
    count = parr->count + 1;
    if(count >  parr->capacity){
        dynarray_realloc(parr,count* sizeof(void *));
    }
    parr->data[count-1] = data;
    parr->count = count;
}

/**
    @brief 初始化动态数组存储容量
    @param parr
    @param initsize
*/
void dynarray_init(DynArray* parr, int initsize){
    if(parr!=NULL){
		parr->data = (void**)malloc(initsize* sizeof(void *));
        parr->count = 0;
        parr->capacity = initsize;
    }
}

/**
    @brief 释放动态数组使用的内存空间
    @param parr
*/
void dynarray_free(DynArray* parr){
    void **p;
    for(p=parr->data;parr->count;p++,parr->count--){
        if(*p)
            free(*p);
    }
    free(parr->data);
    parr->data = NULL;
}

/**
	@brief 计算哈希地址
	@param key	哈希关键字
	@note MAXKEY 哈希表的长度
*/
int elf_hash(char* key){

	int h = 0;
	int g;
	while(*key){
		h = (h<<4)+*key++;
		g = h & 0xf0000000;
		if(g)
			h &=~g;
	}
	return h%MAXKEY;
}


/**
	@brief 运算符、关键字、常量直接放入单词表 和 单词哈希表
	@param tp
	@return tp 返回被添加的单词
*/
TkWord *tkword_direct_insert(TkWord* tp){

	int keyno;
	dynarray_add(&tktable,tp);
	keyno = elf_hash(tp->spelling);
	tp->next = tk_hashtable[keyno];
	tk_hashtable[keyno] = tp;
	return tp;
}

/**
	@brief 在单词表中查找单词
	@param p 要查找单词
	@param keyno	要查找单词的哈希值
*/
TkWord* tkwork_find(char *p,int keyno){
	TkWord * tp = NULL,*tpl;
	for(tpl = tk_hashtable[keyno];tpl;tpl = tpl->next){
		if(!strcmp(p,tpl->spelling)){
			//token = tpl->tkcode;
			tp = tpl;
		}
	}
	return tp;

}

/**
	@brief 分配堆内存并将数据初始化为‘0’
	@param size 分配内存大小
*/
void * mallocz(int size){
	void *ptr;
	ptr = malloc(size);
	if(!ptr && size)
		error("内存分配失败");
	return ptr;
}
/**
	@brief 标示符插入单词表、先查找、查找不到再插入单词表	
	@param p 带插入的标示符
*/
TkWord* tkword_insert(char* p){
	TkWord *tp;
	int keyno;
	char*s;
	char* end;
	int length;
	keyno = elf_hash(p);
	tp = tkwork_find(p,keyno);
	if(tp == NULL){
		length = strlen(p);
		tp = (TkWord*)mallocz(sizeof(TkWord) + length + 1);
		tp->next = tk_hashtable[keyno];
		tk_hashtable[keyno] = tp;
		dynarray_add(&tktable,tp);

		tp->tkcode = tktable.count - 1;
		s = ((char*)tp)+sizeof(tktable);
		tp->spelling = (char*)s;
		for(end = p+length;p<end;){
			* s++ = * p++;
		}
		*s = (char)'\0';
	}
	return tp;
}

/**
	@brief 初始化词法分析器
*/
void init_lex(){
	TkWord* tp;
	static TkWord keywords[] = {
		{TK_PLUS, NULL,"+",NULL,NULL},
		{TK_MINUS,NULL,"-",NULL,NULL},
		{TK_STAR,NULL,"*",NULL,NULL},
		{TK_DIVIDE,NULL,"/",NULL,NULL},
		{TK_MOD,NULL,"%",NULL,NULL},
		{TK_EQ,NULL,"==",NULL,NULL},
		{TK_NEQ,NULL,"!=",NULL,NULL},
		{TK_LT,NULL,"<",NULL,NULL},
		{TK_LEQ,NULL,"<=",NULL,NULL},
		{TK_GT,NULL,">",NULL,NULL},
		
		{TK_GEQ, NULL,">=",NULL,NULL},
		{TK_ASSIGN,NULL,"=",NULL,NULL},
		{TK_POINTSTO,NULL,"->",NULL,NULL},
		{TK_DOT,NULL,".",NULL,NULL},
		{TK_AND,NULL,"&",NULL,NULL},
		{TK_OPENPA,NULL,"(",NULL,NULL},
		{TK_CLOSEPA,NULL,")",NULL,NULL},
		{TK_OPENBK,NULL,"[",NULL,NULL},
		{TK_CLOSEBK,NULL,"]",NULL,NULL},
		{TK_BEGIN,NULL,"{",NULL,NULL},

		{TK_END, NULL,"}",NULL,NULL},
		{TK_SEMICOLON,NULL,";",NULL,NULL},
		{TK_COMMA,NULL,".",NULL,NULL},
		{TK_ELLIPSIS,NULL,"...",NULL,NULL},
		{TK_EOF,NULL,"End_Of_File",NULL,NULL},
		{TK_CINT,NULL,"整型常量",NULL,NULL},
		{TK_CCHAR,NULL,"字符常量",NULL,NULL},
		{TK_CSTR,NULL,"字符串常量",NULL,NULL},
		{KW_CHAR,NULL,"char",NULL,NULL},
		{KW_CHORT,NULL,"short",NULL,NULL},

		{KW_INT, NULL,"int",NULL,NULL},
		{KW_VOID,NULL,"void",NULL,NULL},
		{KW_STRUCT,NULL,"struct",NULL,NULL},
		{KW_IF,NULL,"if",NULL,NULL},
		{KW_ELSE,NULL,"else",NULL,NULL},
		{KW_FOR,NULL,"for",NULL,NULL},
		{KW_CONTINUE,NULL,"continue",NULL,NULL},
		{KW_BREAK,NULL,"break",NULL,NULL},
		{KW_RETURN,NULL,"return",NULL,NULL},
		{KW_SIZEOF,NULL,"sizeof",NULL,NULL},

		{KW_ALIGN,NULL,"__align",NULL,NULL},
		{KW_CDECL,NULL,"__cdecl",NULL,NULL},
		{KW_STDCALL,NULL,"__stdcall",NULL,NULL},
		{0,NULL,NULL,NULL,NULL}
		
	};

	
	dynarray_init(&tktable,8);
	for(tp = &keywords[0];tp->spelling!=NULL;tp++)
		tkword_direct_insert(tp);
}


/**
	@brief 取单词
*/
void get_token(){
	preprocess();
	if('a'<=ch && ch<='z' || 'A'<=ch && ch<='Z' || ch == '_'){
		
		TkWord *tp;
		parse_identifier();
		tp = tkword_insert(tkstr.data);
		token = tp->tkcode;

	}else if('0'<=ch && ch <='9'){
	
		parse_num();
		token = TK_CINT;

	}else if('+' == ch){

		getch();
		token = TK_PLUS;
	
	}else if('-' == ch){
	
		getch();
		if(ch == '>'){
			token = TK_POINTSTO;
			getch();
		}else{
			token = TK_MINUS;
		}

	}else if('/' == ch){
	
		token = TK_DIVIDE;
		getch();

	}else if('%' == ch){
	
		token = TK_MOD;
		getch();

	}else if('=' == ch){
	
		getch();
		if('='==ch){
			token = TK_EQ;
			getch();
		}else{
			token = TK_ASSIGN;
		}

	}else if('!' == ch){

		getch();
		if('=' == ch){
			token = TK_NEQ;
			getch();
		}else{
			error("暂不支持!操作符");
		}
	
	}else if('<' == ch){

		getch();
		if('=' == ch){
			token =TK_LEQ;
			getch();
		}else{
			token = TK_LT;
		}
	
	}else if('>' == ch){
	
		getch();
		if('=' == ch){
			token = TK_GEQ;
			getch();
		}else{
			token = TK_LT;
		}

	}else if('.' == ch){
	
		getch();
		if('.' == ch){
			getch();
			if('.'== ch){
				error("省略号拼写错误");
			}else{
				token = TK_ELLIPSIS;
			}
		}else{
			token = TK_DOT;
		}

	}else if('&' == ch){
	
		token = TK_AND;
		getch();

	}else if(';' == ch){
		token = TK_SEMICOLON;
		getch();

	}else if(']' == ch){
		token = TK_CLOSEBK;
		getch();
	}else if('}' == ch){
		token = TK_END;
		getch();
	}else if(')' == ch){
		token = TK_CLOSEPA;
		getch();
	}else if('[' == ch){
		token = TK_OPENBK;
		getch();
	}else if('{' == ch){
		token = TK_BEGIN;
		getch();
	}else if(',' == ch){
		token = TK_COMMA;
		getch();
	}else if('(' == ch){
		token = TK_OPENPA;
		getch();
	}else if('*' == ch){
		token = TK_STAR;
		getch();
	}else if('\'' == ch){
		parse_string(ch);
		token = TK_CCHAR;
		tkvalue = * (char*)tkstr.data;
	}else if('\"' == ch){
		parse_string(ch);
		token = TK_CSTR;
	}else if(EOF == ch){
		token = TK_EOF;
	}else{
		error("不认识的字符：\\x%02x",ch);
		getch();
	}
}

/**
	@brief 从源程序中读取一个字符
*/
void getch(){

	ch = getc(fin);
}

/**
	@brief 预处理，忽略空白符及注释
*/
void preprocess(){

	while(1){
		if(' ' == ch || '\t' == ch || '\r' == ch){
			skip_while_space();
		}else if('/' == ch){
			getch();
			if(ch == '*'){
				parse_comment();
			}else{
				ungetc(ch,fin);
			}
		}else{
			break;
		}

	}
}

/**
	@brief 解析注释
*/
void parse_comment(){

	getch();
	do{
	
		do{
			if(ch == '\n' || ch == '*' || ch == EOF)
				break;
			else 
				getch();
		}while(1);

		if('\n' == ch){
			line_num ++;
		}else if(ch == '*'){
			getch();
			if('/' == ch){
				getch();
				return;
			}
		}else{
			error("文件尾没有看到配对的注释结束符");
			return;
		}

	}while(1);
}

/**
	@brief 忽略空格、Tab和回车
*/
void skip_while_space(){
	while(ch == ' ' || ch == '\t' || ch == '\r'){
		if('\r' == ch ){
			getch();
			if(ch != '\n')
				return;
			line_num ++;
		}
		printf("%c",ch);
		getch();
	}
}

/**
	@brief 判断c是否为字母或下划线
	@param c 字符
*/
int is_nodigit(char c){
	return (c >= 'a' && c<='z') ||
		(c >= 'A' && c<= 'Z') ||
		c == '_';
}

/**
	@brief 判断c是否是数字
	@param c 字符
*/
int is_digit(char c){
	return c>='0' && c<='9';
}

/**
	@brief 解析标示符
*/
void parse_identifier(){
	dynstring_reset(&tkstr);
	dynstring_chcat(&tkstr,ch);
	getch();
	while(is_nodigit(ch) || is_digit(ch)){
		dynstring_chcat(&tkstr,ch);
		getch();
	}
	dynstring_chcat(&tkstr,'\0');
}

/**
	@brief 解析整型常量
*/
void parse_num(){
	dynstring_reset(&tkstr);
	dynstring_reset(&sourcestr);
	do{
		dynstring_chcat(&tkstr,ch);
		dynstring_chcat(&sourcestr,ch);
		getch();
	}while(is_digit(ch));

	if('.' == ch){
		do{
			dynstring_chcat(&tkstr,ch);
			dynstring_chcat(&sourcestr,ch);
			getch();
		}while(is_digit(ch));
	}

	dynstring_chcat(&tkstr,'\0');
	dynstring_chcat(&sourcestr,'\0');
	tkvalue = atoi(tkstr.data);
}

/**
	@brief 解析字符常量、字符串常量
	@param sep 字符常量界符标识为单引号‘
				字符串常量界符标识为双引号“
*/
void parse_string(char sep){
	char c;
	dynstring_reset(&tkstr);
	dynstring_reset(&sourcestr);
	dynstring_chcat(&sourcestr,sep);
	getch();
	for(;;){
		if(ch == sep){
			break;
		}else if('\\' == ch){
			dynstring_chcat(&sourcestr,ch);
			getch();
			switch (ch)
			{
			case '0':
				c = '\0';
				break;
			case 'a':
				c = '\a';
				break;
			case 'b':
				c = '\b';
				break;
			case 't':
				c = '\t';
				break;
			case 'n':
				c = '\n';
				break;
			case 'v':
				c = '\v';
				break;
			case 'f':
				c = '\f';
				break;
			case 'r':
				c = '\r';
				break;
			case '\"':
				c = '\"';
				break;
			case '\'':
				c = '\'';
				break;
			case '\\':
				c = '\\';
				break;
			default:
				c = ch;
				if(c>='!' && c<='~')
					warning("非法转义字符:\'\\%c\'",c);
				else
					warning("非法转义字符:\'\\0x%x\'",c);
				break;
			}
			dynstring_chcat(&tkstr,c);
			dynstring_chcat(&sourcestr,ch);
			getch();
		}else{
			dynstring_chcat(&tkstr,ch);
			dynstring_chcat(&sourcestr,ch);
			getch();
		}
	}
	dynstring_chcat(&tkstr,'\0');
	dynstring_chcat(&sourcestr,sep);
	dynstring_chcat(&sourcestr,'\0');
	getch();
}

/**
	@brief 词法着色
*/
void color_token(int lex_state){
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	char *p;
	switch (lex_state)
	{
	case LEX_NORMAL:
		{
			if(token >= TK_IDENT){
				SetConsoleTextAttribute(h,FOREGROUND_INTENSITY);
			}else if(token >= KW_CHAR){
				SetConsoleTextAttribute(h,FOREGROUND_GREEN|FOREGROUND_INTENSITY);
			}else if(token >= TK_CINT){
				SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_GREEN);
			}else{
				SetConsoleTextAttribute(h,FOREGROUND_RED|FOREGROUND_INTENSITY);
			}
			p = get_tkstr(token);
			printf("%s",p);
			break;
		}
	case LEX_SEP:
		{
			printf("%c",ch);
		}
	default:
		break;
	}
}

/**
	@brief 词法分析器初始化部分
*/
void init(){
	line_num = 1;
	init_lex();
}

/**
	@brief 词法分析器扫尾清理工作
*/
void cleanup(){
	int i;
	printf("\ntktable.cout=",tktable.count);
	dynarray_free(&tktable);
	/*for(i = 0;i<tktable.count;++i){
		free(tktable.data[i]);
	}
	free(tktable.data);*/
}