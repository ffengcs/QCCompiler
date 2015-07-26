#ifndef PARSER_H
#define PARSER_H

/**
	@brief 翻译单元
*/
void translation_unit();

/**
	@brief 解析外部命令
	@param l 存储类型，局部的or全局的
*/
void external_declaration(int l);

/**
	@brief 解析类型区分符
	@return 是否发现合法的类型区分符
*/
int type_specifier();

/**
	@brief 解析结构区分符
*/
void struct_specifier();

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
	@brief 语法状态枚举值
*/
enum e_SynTaxState{
	SNTX_NUL,
	SNTX_SP,
	SNTX_LE_HT,
	SNTX_DELAY
};

/**
	@brief 定义变量时全局的还是局部的
*/
enum e_Scope
{
	SC_GLOBAL,
	SC_LOCAL
};

/**
	@brief 语法缩进功能
*/
void syntax_indent();

/**
	@brief 缩进n个tab键
	@param n 缩进tab键个数
*/
void print_Tab(int n);

#endif