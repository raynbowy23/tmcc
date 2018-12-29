#include "tmcc.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// void program();
// Node *assign();
// Node *expr();
// Node *mul();
// Node *term();

static Vector *tokens;
static int pos;

static void expect(int ty){
    Token *token = tokens->data[pos];
    if(token->ty != ty)
        error("%c (%d) expected, but got %c (%d)", ty, ty, token->ty, token->ty);
    pos++;
}

/*
program: assign program'
program': ε | assign program'
*/
// void program(){
//     while(((Token *)(tokens->data[pos]))->ty != TK_EOF)
//         vec_push(code, assign());
// }

/*
assign: expr assign' ";"
assign': ε | "="  expr assign'
assign':     "==" expr assign'
assign':     "!=" expr assign'
*/
// Node *assign(){
//     Node *lhs = expr();

//     if(((Token *)tokens->data[pos])->ty == '='){
//         pos++;
//         return new_node('=', lhs, assign());
//     }
    
//     if(((Token *)tokens->data[pos])->ty == TK_EQ){
//         pos++;
//         return new_node(ND_EQ, lhs, assign());
//     }
    
//     if(((Token *)tokens->data[pos])->ty == TK_NE){
//         pos++;
//         return new_node(ND_NE, lhs, assign());
//     }

//     if(((Token *)tokens->data[pos])->ty == TK_AND){
//         pos++;
//         return new_node(ND_AND, lhs, assign());
//     }

//     if(((Token *)tokens->data[pos])->ty == TK_OR){
//         pos++;
//         return new_node(ND_OR, lhs, assign());
//     }

//     if(((Token *)tokens->data[pos])->ty == ';'){
//         pos++;
//         return lhs;
//     }

    // if(((Token *) tokens->data[pos])->ty != ';'){
    //     error(";で文が終わってない: %s", ((Token *)tokens->data[pos])->input);
    // }

//     error("unexpected token: %s\n", ((Token *)tokens->data[pos])->input);
// }

static Node *new_node(int op, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->ty = op;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

/*
term: NUMBER | IDENT | "(" expr ")"
 */
Node *term() {
    Token *token = tokens->data[pos];
    if (token->ty != TK_NUM)
        error("number expected, but got %s", token->input);
    pos++;
        // return new_node_num(token->val);
    // if (token->ty == TK_IDENT){
    //     if(map_get(var_tab, token->name) == NULL){
    //         int *num = (int *)malloc(sizeof(int));
    //         *num = var_cnt++;
    //         map_put(var_tab, token->name, (void *)num);
    //     }
    //     pos++;
    //     return new_node_ident(token->name);
    // }
    // if (token->ty == '(') {
    //     pos++;
    //     Node *node = expr();
    //     if (token->ty != ')')
    //         error("開き括弧と閉じ括弧の対応がついてないです: %s", ((Token *)tokens->data[pos])->input);
    //     pos++;
    //     return node;
    //  }

    Node *node = malloc(sizeof(Node));
    node->ty = ND_NUM;
    node->val = token->val;
    return node;
    //  error("数値でも開きカッコでもないトークンです: %s", ((Token *)tokens->data[pos])->input);
}
/*
mul: term | term "*" mul | term "/" mul
--
mul: term ( "*" mul | "/" mul | ε )
 */
static Node *mul(){
    Node *lhs = term();
    for(;;){
        Token *token = tokens->data[pos];
        int op = token->ty;
        if(op != '*' && op != '/' && op != '%')
            return lhs;
        pos++;
        lhs = new_node(op, lhs, term());
    }

    // if (((Token *)tokens->data[pos])->ty == ('*')) {
    //     pos++;
    //     return new_node('*', lhs, mul());
    // }
    // if (((Token *)tokens->data[pos])->ty == ('/')) {
    //     pos++;
    //     return new_node('/', lhs, mul());
    // }
    // if (((Token *)tokens->data[pos])->ty == ('%')) {
    //     pos++;
    //     return new_node('%', lhs, mul());
    // }

}

/*
expr: mul | mul "+" expr | mul "-" expr
--
expr: mul ( "+" expr | "-" expr | ε )
*/
static Node *expr(){
    Node *lhs = mul();
    for(;;){
        Token *token = tokens->data[pos];
        int op = token->ty;
        if(op != '+' && op != '-')
            return lhs;
        pos++;
        lhs = new_node(op, lhs, mul());
    }
}


static Node *stmt(){
    Node *node = malloc(sizeof(Node));
    node->ty = ND_COMP_STMT;
    node->stmts = new_vector();

    for(;;){
        Token *token = tokens->data[pos];
        if(token->ty == TK_EOF)
            return node;

        Node *e = malloc(sizeof(Node));

        if(token->ty == TK_RETURN){
            pos++;
            e->ty = ND_RETURN;
            e->expr = expr();
        }else{
            e->ty = ND_EXPR_STMT;
            e->expr = expr();
        }

        vec_push(node->stmts, e);
        expect(';');
    }
}

Node *parse(Vector *vec){
    tokens = vec;
    pos = 0;

    return stmt();
}