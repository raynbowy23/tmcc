#include "tmcc.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void program();
Node *assign();
Node *expr();
Node *mul();
Node *term();

void parse() { program();}

/*
program: assign program'
program': ε | assign program'
*/
void program(){
    while(((Token *)(tokens->data[pos]))->ty != TK_EOF)
        vec_push(code, assign());
}

/*
assign: expr assign' ";"
assign': ε | "="  expr assign'
assign':     "==" expr assign'
assign':     "!=" expr assign'
*/
Node *assign(){
    Node *lhs = expr();

    if(((Token *)tokens->data[pos])->ty == '='){
        pos++;
        return new_node('=', lhs, assign());
    }
    
    if(((Token *)tokens->data[pos])->ty == TK_EQ){
        pos++;
        return new_node(ND_EQ, lhs, assign());
    }
    
    if(((Token *)tokens->data[pos])->ty == TK_NE){
        pos++;
        return new_node(ND_NE, lhs, assign());
    }

    if(((Token *)tokens->data[pos])->ty == ';'){
        pos++;
        return lhs;
    }

    // if(((Token *) tokens->data[pos])->ty != ';'){
    //     error(";で文が終わってない: %s", ((Token *)tokens->data[pos])->input);
    // }

    error("unexpected token: %s\n", ((Token *)tokens->data[pos])->input);
}
/*
expr: mul | mul "+" expr | mul "-" expr
--
expr: mul ( "+" expr | "-" expr | ε )
*/
Node *expr(){
    Node *lhs = mul();
    if(((Token *)tokens->data[pos])->ty == TK_EOF || ((Token *)tokens->data[pos])->ty == ')' || ((Token *)tokens->data[pos])->ty == ';' || ((Token *)tokens->data[pos])->ty == '=')
        return lhs;
    if(((Token *)tokens->data[pos])->ty == '+'){
        pos++;
        return new_node('+', lhs, expr());
    }
    if(((Token *)tokens->data[pos])->ty == '-'){
        pos++;
        return new_node('-', lhs, expr());
    }
    return lhs;
}

/*
mul: term | term "*" mul | term "/" mul
--
mul: term ( "*" mul | "/" mul | ε )
 */
Node *mul(){
    Node *lhs = term();

    if (((Token *)tokens->data[pos])->ty == ('*')) {
        pos++;
        return new_node('*', lhs, mul());
    }
    if (((Token *)tokens->data[pos])->ty == ('/')) {
        pos++;
        return new_node('/', lhs, mul());
    }
    if (((Token *)tokens->data[pos])->ty == ('%')) {
        pos++;
        return new_node('%', lhs, mul());
    }

    return lhs;
}

/*
term: NUMBER | IDENT | "(" expr ")"
 */
Node *term() {
    if (((Token *)tokens->data[pos])->ty == TK_NUM)
        return new_node_num(((Token *)tokens->data[pos++])->val);
    if (((Token *)tokens->data[pos])->ty == TK_IDENT){
        if(map_get(var_tab, ((Token *)tokens->data[pos])->name) == NULL){
            int *num = (int *)malloc(sizeof(int));
            *num = var_cnt++;
            map_put(var_tab, ((Token *)tokens->data[pos])->name, (void *)num);
        }
        return new_node_ident(((Token *)tokens->data[pos++])->name);
    }
    if (((Token *)tokens->data[pos])->ty == '(') {
        pos++;
        Node *node = expr();
        if (((Token *)tokens->data[pos])->ty != ')')
            error("開き括弧と閉じ括弧の対応がついてないです: %s", ((Token *)tokens->data[pos])->input);
        pos++;
        return node;
     }
     error("数値でも開きカッコでもないトークンです: %s", ((Token *)tokens->data[pos])->input);
}
