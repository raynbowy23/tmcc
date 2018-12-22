#include "tmcc.h"
#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//pが指している文字列をトークンに分割してtokensに保存する
void tokenize(char *p){
    Token *token;

    while(*p){
        //空白文字をスキップ
        if(isspace(*p)){
            p++;
            continue;
        }

        token = malloc(sizeof(Token));

        switch(*p){
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '(':
        case ')':
        case ';':
            token->ty = *p;
            token->input = p;
            vec_push(tokens, token);
            p++;
            continue;
        }

        // "=", "=="
        if(*p == '='){
            if(*(p+1) == '='){
                token->ty = TK_EQ;
                token->input = p;
                vec_push(tokens, token);
                p += 2;
            }else{ 
                token->ty = *p;
                token->input = p;
                vec_push(tokens, token);
                p++;
            }
            continue;
        }

        // "!="
        if(*p == '!' && *(p+1) == '='){
            token->ty = TK_NE;
            token->input = p;
            vec_push(tokens, token);
            p += 2;
            continue;
        }

        if(isdigit(*p)){
            token->ty = TK_NUM;
            token->input = p;
            token->val = strtol(p, &p, 10);
            vec_push(tokens, token);
            continue;
        }

        //アルファベットの小文字ならば、TK_IDENT型のトークンを作成
        if(isalpha(*p)){

            int len = 1;
            for(char *q = p + 1; isalnum(*q); q++)
                len++;
            token->name = malloc(sizeof(char) * (len+1));
            strncpy(token->name, p, len+1);
            token->name[len] = '\0';

            token->ty = TK_IDENT;
            token->input = p;
            vec_push(tokens, token);
            p += len;
            continue;
        }

        fprintf(stderr, "トークナイズできません: %s\n", p);
        exit(1);
    }
    token = malloc(sizeof(Token));
    token->ty = TK_EOF;
    token->input = p;
    vec_push(tokens, token);
}