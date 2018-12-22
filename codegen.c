#include "tmcc.h"
#include "util.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void gen_lval(Node *node){
    if(node->ty == ND_IDENT){
        int *num = (int *)map_get(var_tab, node->name);
        printf("    mov rax, rbp\n");
        printf("    sub rax, %d\n", (*num + 1) * 8);
        printf("    push rax\n");
        return;
    }
    error("代入の左辺値が変数ではありません");
}

void gen(Node *node){
    if(node->ty == ND_NUM){
        printf("    push %d\n", node->val);
        return;
    }

    if(node->ty == ND_IDENT){
        gen_lval(node);
        printf("    pop rax\n");
        printf("    mov rax, [rax]\n");
        printf("    push rax\n");
        return;
    }

    if(node->ty == '='){
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("    pop rdi\n");
        printf("    pop rax\n");
        printf("    mov [rax], rdi\n");
        printf("    push rdi\n");
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch(node->ty){
    case '+':
        printf("    add rax, rdi\n");
        break;
    case '-':
        printf("    sub rax, rdi\n");
        break;
    case '*':
        printf("    mul rdi\n");
        break;
    case '/':
        printf("    mov rdx, 0\n");
        printf("    div rdi\n");
        break;
    case '%':
        printf("    mov rdx, 0\n");
        printf("    div rdi\n");
        printf("    mov rax, rdx\n");
        break;
    case ND_EQ:
        printf("    cmp rdi, rax\n");
        printf("    sete al\n");
        printf("    movzb rax, al\n");
        break;
    case ND_NE:
        printf("    cmp rdi, rax\n");
        printf("    setne al\n");
        printf("    movzb rax, al\n"); //al代入して上位56ビットは0クリア
        break;
    case ND_AND:
        printf("    and rdi, rax\n");
        printf("    mov rax, rdi\n");
        break;
    case ND_OR:
        printf("    or rdi, rax\n");
        printf("    mov rax, rdi\n");
        break;
    default:
        error("unexpected type of node\n");
    }

    printf("    push rax\n");
}
