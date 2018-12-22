#include "util.h"

//トークンの型を表す値
enum{
    TK_NUM = 256, //整数トークン
    TK_IDENT,     //識別子
    TK_EQ,        // '=='
    TK_NE,        // '!='
    TK_AND,       // '&&'
    TK_OR,        // '||'
    TK_EOF,       //入力の終わりを表すトークン
};

//トークンの型
typedef struct{
    int ty;      //トークンの型
    int val;     //tyがtk_numの場合、その数値
    char *name;  //tyがTK_IDENTの場合その名前
    char *input; //トークン文字列（エラーメッセージ用）
}Token;

extern Vector *tokens;
extern int pos;
void tokenize(char *);

enum{
    ND_NUM = 256,   //整数のノードの型
    ND_IDENT,       //識別子のノードの型
    ND_EQ,          // '=='
    ND_NE,          // '!='
    ND_AND,         // '&&'
    ND_OR,          // '||'
};

typedef struct Node{
    int ty;             //演算子かnd_num
    struct Node *lhs;   //左辺
    struct Node *rhs;   //右辺
    int val;            //tyがnd_numの場合のみ使う
    char *name;          //tyがnd_identの場合のみ使う
}Node;

extern Vector *code;

Node *new_node(int, Node *, Node *);
Node *new_node_num(int);
Node *new_node_ident(char *name);

void parse();
void gen(Node *);

extern Map *var_tab;
extern int var_cnt;

//debug.c
void p_tree(Node *);