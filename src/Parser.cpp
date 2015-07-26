#include"Parser.h"
#include"GlobalVar.h"
#include<stdio.h>

/**
	@brief 翻译单元
*/
void translation_unit(){

	while(token != TK_EOF){
		external_declaration(SC_GLOBAL);
	}
}

/**
	@brief 解析外部命令
	@param l 存储类型，局部的or全局的
*/
void external_declaration(int l){
	if(!type_specifier()){
		expect("<类型区分符>");
	}
	if(token == TK_SEMICOLON){
		get_token();
		return;
	}
	while(1){
		declarator();
		if(token == TK_BEGIN){
			if(l == SC_LOCAL){
				error("不支持函数嵌套定义");
			}
			funcbody();
			break;
		}else{
			if(token == TK_ASSIGN){
				get_token();
				initializer();
			}
			if(token == TK_COMMA){
				get_token();
			}else{
				syntax_state = SNTX_LE_HT;
				skip(TK_SEMICOLON);
				break;
			}
		}
	}
}

/**
	@brief 解析类型区分符
	@return 是否发现合法的类型区分符
*/
int type_specifier(){
	int type_found = 0;
	switch (token)
	{
	case KW_CHAR:
		/*type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;*/
	case KW_CHORT:
		/*type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;*/
	case KW_VOID:
		/*type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;*/
	case KW_INT:
		type_found = 1;
		syntax_state = SNTX_SP;
		get_token();
		break;
	case KW_STRUCT:
		syntax_state = SNTX_SP;
		struct_specifier();
		type_found = 1;
		break;
	default:
		break;
	}
	return type_found;
}

/**
	@brief 解析结构区分符
*/
void struct_specifier(){
	
}

/**
	@brief 解析结构声明符表
*/
void struct_declaration_list();

/**
	@brief 解析结构声明
*/
void struct_declaration();

/**
	@brief 解析函数声明
*/
void function_calling_convention(int *fc);

/**
	@brief 数据结构成员对齐
*/
void struct_member_alignment();

/**
	@brief 解析声明符
*/
void declarator();

/**
	@bried 解析直接声明符
*/
void direct_declarator();

/**
	@brief 解析直接声明符后缀
*/
void direct_declarator_postfix();

/**
	@brief 解析形参类型表
	@param func_call 函数调用约定
*/
void parameter_type_list(int func_call);

/**
	@brief 解析函数体代码
*/
void funcbody();

/**
	@brief 解析初值符
*/
void initializer();

/**
	@brief 解析语句
	@param bsym
	@param csym
*/
void statement(int * bsym,int *csym);

/**
	@brief 解析符合语句
*/
void compound_statement();

/**
	@brief 判断是否为类型区分符
	@param v 单词编号
*/
int is_type_specifier(int v);

/**
	@brief 表达式相关的代码
*/
void expression_statement();

/**
	@brief 选择语句
*/
void if_statement();

/**
	@brief 循环语句
*/
void for_statement();

/**
	@brief continue 语句
*/
void continue_statement();

/**
	@brief break语句
*/
void break_statement();

/**
	@brief return语句
*/
void return_statement();

/**
	@brief 表达式
*/
void expression();

/**
	@brief 赋值表达式
*/
void assignment_expression();

/**
	@brief 相等类的表达式
*/
void equality_expression();

/**
	@brief 关系表达式
*/
void relational_expression();

/**
	@brief 加法类的表达式
*/
void additive_expression();

/**
	@brief 乘除法类表达式
*/
void multiplicative_expression();

/**
	@brief 一元表达式
*/
void unary_expression();

/**
	@brief sizeof表达式
*/
void sizeof_expression();

/**
	@brief 后缀表达式
*/
void postfix_expression();

/**
	@brief 初值表达式
*/
void primary_expression();

/**
	@brief 实参表达式
*/
void argument_expression_list();

/**
	@brief 语法缩进功能
*/
void syntax_indent(){
	switch (syntax_state)
	{
	case SNTX_NUL:
		color_token(LEX_NORMAL);
		break;
	case SNTX_SP:
		printf(" ");
		color_token(LEX_NORMAL);
		break;
	case SNTX_LE_HT:
		{
			if(token == TK_END)
				syntax_level -- ;
			printf("\n");
			print_Tab(syntax_level);
		}
		color_token(LEX_NORMAL);
		break;
	case SNTX_DELAY:
		break;
	default:
		break;
	}
	syntax_state = SNTX_NUL;
}

/**
	@brief 缩进n个tab键
	@param n 缩进tab键个数
*/
void print_Tab(int n){
	int i = 0;
	for(;i<n;i++){
		printf("\t");
	}
}