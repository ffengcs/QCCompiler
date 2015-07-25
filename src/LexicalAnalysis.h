#ifndef LEXICAl_ANALYSIS_H
#define LEXICAl_ANALYSIS_H

/*
	@brief 哈希表的长度
*/
#define MAXKEY 1024


/*
	@brief 单词编码
*/
enum e_TokenCode{

	TK_PLUS,    //+
	TK_MINUS,   //-
	TK_STAR,    //*
	TK_DIVIDE,  ///
    TK_MOD,     //%
    TK_EQ,      //==
    TK_NEQ,     //!=
    TK_LT,      //<
    TK_LEQ,     //<=
    TK_GT,      //>
    TK_GEQ,     //>=
    TK_ASSIGN,  //=
    TK_POINTSTO,//->
    TK_DOT,     //.
    TK_AND,     //&
    TK_OPENPA,  //(
    TK_CLOSEPA, //)
    TK_OPENBK,  //[
    TK_CLOSEBK, //]
    TK_BEGIN,   //{
    TK_END,     //}
    TK_SEMICOLON,//;
    TK_COMMA,   //,
    TK_ELLIPSIS,//...
    TK_EOF,     //文件结束符

    /*常量*/
    TK_CINT,
    TK_CCHAR,
    TK_CSTR,

    /*关键字*/
    KW_CHAR,
    KW_CHORT,
    KW_INT,
    KW_VOID,
    KW_STRUCT,
    KW_IF,
    KW_ELSE,
    KW_FOR,
    KW_CONTINUE,
    KW_BREAK,
    KW_RETURN,
    KW_SIZEOF,

    KW_ALIGN,
    KW_CDECL,
    KW_STDCALL,

    /*标示符*/
    TK_IDENT,

};

/*
    @brief 动态字符串的定义
*/
typedef struct DynString{
    int count;
    int capacity;
    char* data;
}DynString;

/**
    @brief 动态数组定义
*/
typedef struct DynArray{

    int count;
    int capacity;
    void ** data;
}DynArray;



/**
	@brief 单词表
*/
typedef struct TkWord
{
	int tkcode;
	struct TkWord* next;
	char * spelling;
	struct Symbol * sym_struct;
	struct Symbol * sym_identifier;
}TkWord;





/********下面是词法分析过程*********/

/**
	@brief 取单词
*/
void get_token();

/**
	@brief 从源程序中读取一个字符
*/
void getch();

/**
	@brief 预处理，忽略空白符及注释
*/
void preprocess();

/**
	@brief 解析注释
*/
void parse_comment();

/**
	@brief 忽略空格、Tab和回车
*/
void skip_while_space();

/**
	@brief 判断c是否为字母或下划线
	@param c 字符
*/
int is_nodigit(char c);

/**
	@brief 判断c是否是数字
	@param c 字符
*/
int is_digit(char c);

/**
	@brief 解析标示符
*/
void parse_identifier();

/**
	@brief 解析整型常量
*/
void parse_num();

/**
	@brief 解析字符常量、字符串常量
	@param sep 字符常量界符标识为单引号‘
				字符串常量界符标识为双引号“
*/
void parse_string(char sep);

/**
	@brief 词法着色
*/
void color_token(int lex_state);

/**
	@brief 词法状态枚举定义
*/
enum e_LexState{
	LEX_NORMAL,
	LEX_SEP
};

/**
	@brief 词法分析器初始化部分
*/
void init();

/**
	@brief 词法分析器扫尾清理工作
*/
void cleanup();

#endif
