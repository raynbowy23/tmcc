#include "tmcc.h"
#include "util.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Map *keywords;

//Tokenizer
static Token *add_token(Vector *vec, int ty, char *input){
    Token *token = malloc(sizeof(Token));
    token->ty = ty;
    token->input = input;
    vec_push(vec, token);
    return token;
}

//pが指している文字列をトークンに分割してtokensに保存する
// void tokenize(char *p){
static Vector *scan(char *p){
    Vector *vec = new_vector();

    while(*p){

        //空白文字をスキップ
        if(isspace(*p)){
            p++;
            continue;
        }

        if(strchr("+-*/\%;=", *p)){
            add_token(vec, *p, p);
            p++;
            continue;
        }
        // switch(*p){
        // case '+':
        // case '-':
        // case '*':
        // case '/':
        // case '%':
        // case '(':
        // case ')':
        // case ';':
        //     token->ty = *p;
        //     token->input = p;
        //     vec_push(tokens, token);
        //     p++;
        //     continue;
        // }

        // "=", "=="
        // if(*p == '='){
        //     if(*(p+1) == '='){
        //         add_token(vec, TK_EQ, p);
        //         p += 2;
        //     }else{ 
        //         add_token(vec, *p, p);
        //         p++;
        //     }
        //     continue;
        // }

        // // "!="
        // if(*p == '!' && *(p+1) == '='){
        //     add_token(vec, TK_NE, p);
        //     p += 2;
        //     continue;
        // }

        // // "&&"
        // if(*p == '&' && *(p+1) == '&'){
        //     add_token(vec, TK_AND, p);
        //     p += 2;
        //     continue;
        // }

        // // "||"
        // if(*p == '|' && *(p+1) == '|'){
        //     add_token(vec, TK_OR, p);
        //     p += 2;
        //     continue;
        // }

        //Identifier
        //アルファベットの小文字ならば、TK_IDENT型のトークンを作成
        if(isalpha(*p) || *p == '_'){
            int len = 1;
            while(isalpha(p[len]) || isdigit(p[len]) || p[len] == '_')
                len++;
            // for(char *q = p + 1; isalnum(*q); q++)
            //     len++;
            // token->name = malloc(sizeof(char) * (len+1));
            // strncpy(token->name, p, len+1);
            // token->name[len] = '\0';
            char *name = strndup(p, len);
            int ty = (intptr_t)map_get(keywords, name);
            if(!ty)
                ty = TK_IDENT;

            //add_token
            Token *token = add_token(vec, ty, p);
            token->name = name;
            p += len;
            continue;
        }

        if(isdigit(*p)){
            Token *token = add_token(vec, TK_NUM, p);
            token->val = strtol(p, &p, 10);
            continue;
        }

        // fprintf(stderr, "トークナイズできません: %s\n", p);
        // exit(1);
        error("cannot tokenize: %s", p);
    }
    add_token(vec, TK_EOF, p);
    return vec;
}

Vector *tokenize(char *p){
    keywords = new_map();
    map_put(keywords, "return", (void *)TK_RETURN);

    return scan(p);
}