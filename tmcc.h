#define _GNU_SOURCE
#include "util.h"
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

// token.c
//トークンの型を表す値
enum{
    TK_NUM = 256, //整数トークン
    TK_IDENT,     //識別子
    // TK_EQ,        // '=='
    // TK_NE,        // '!='
    // TK_AND,       // '&&'
    // TK_OR,        // '||'
    TK_RETURN,    // 'return'
    TK_EOF,       //入力の終わりを表すトークン
};

//トークンの型
typedef struct{
    int ty;      //トークンの型
    int val;     //tyがtk_numの場合、その数値
    char *name;  //tyがTK_IDENTの場合その名前
    char *input; //トークン文字列（エラーメッセージ用）
}Token;

Vector *tokenize(char *p);

// parse.c
enum{
    ND_NUM = 256,   //整数のノードの型
    ND_IDENT,       //識別子のノードの型
    // ND_EQ,          // '=='
    // ND_NE,          // '!='
    // ND_AND,         // '&&'
    // ND_OR,          // '||'
    ND_RETURN,      // Return statement
    ND_COMP_STMT,   // Compound statement
    ND_EXPR_STMT,   // Expression statement
};

typedef struct Node{
    int ty;             //演算子かnd_num
    struct Node *lhs;   //左辺
    struct Node *rhs;   //右辺
    int val;            //tyがnd_numの場合のみ使う
    char *name;          //tyがnd_identの場合のみ使う
    struct Node *expr;  //"return" or expression stmt
    Vector *stmts;      //Compund statement
}Node;

Node *parse(Vector *tokens);

// ir.c

enum{
    IR_IMM,     //即値
    IR_MOV,
    IR_RETURN,
    IR_ALLOCA,
    IR_LOAD,
    IR_STORE,
    IR_KILL,
    IR_NOP,
};

typedef struct{
    int op;
    int lhs;
    int rhs;
}IR;

Vector *gen_ir(Node *node);


// Node *new_node(int, Node *, Node *);
// Node *new_node_num(int);
// Node *new_node_ident(char *name);

// void parse();
// void gen(Node *);

// extern Map *var_tab;
// extern int var_cnt;

//debug.c
// void p_tree(Node *);

//regalloc.c
extern char *regs[];
void alloc_regs(Vector *irv);

//codegen.c
void gen_x86(Vector *irv);