#include "tmcc.h"
#include "util.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// void gen_lval(Node *node){
//     if(node->ty == ND_IDENT){
//         int *num = (int *)map_get(var_tab, node->name);
//         printf("    mov rax, rbp\n");
//         printf("    sub rax, %d\n", (*num + 1) * 8);
//         printf("    push rax\n");
//         return;
//     }
//     error("代入の左辺値が変数ではありません");
// }

void gen_x86(Vector *irv){
    
    for(int i=0; i<irv->len; i++){

        // if(ir->op == ND_NUM){
        //     printf("    push %d\n", irv->val);
        //     return;
        // }

        // if(ir->ty == ND_IDENT){
        //     gen_lval(node);
        //     printf("    pop rax\n");
        //     printf("    mov rax, [rax]\n");
        //     printf("    push rax\n");
        //     return;
        // }

        // if(node->ty == '='){
        //     gen_lval(node->lhs);
        //     gen(node->rhs);

        //     printf("    pop rdi\n");
        //     printf("    pop rax\n");
        //     printf("    mov [rax], rdi\n");
        //     printf("    push rdi\n");
        //     return;
        // }

        // gen(node->lhs);
        // gen(node->rhs);

        // printf("    pop rdi\n");
        // printf("    pop rax\n");

        IR *ir = irv->data[i];

        switch(ir->op){
        case IR_IMM:
            printf("    mov %s, %d\n", regs[ir->lhs], ir->rhs);
            break;
        case IR_MOV:
            printf("    mov %s, %s\n", regs[ir->lhs], regs[ir->rhs]);
            break;
        case IR_RETURN:
            printf("    mov rax, %s\n", regs[ir->lhs]);
            printf("    ret\n");
            break;
        case '+':
            printf("    add %s, %s\n", regs[ir->lhs], regs[ir->rhs]);
            break;
        case '-':
            printf("    sub %s, %s\n", regs[ir->lhs], regs[ir->rhs]);
            break;
        case '*':
            printf("    mov rax, %s\n", regs[ir->rhs]);
            printf("    mul %s\n", regs[ir->lhs]);
            printf("    mov %s, rax\n", regs[ir->lhs]);
            break;
        case '/':
            printf("    mov rax, %s\n", regs[ir->lhs]);
            printf("    cqo\n");
            printf("    div %s\n", regs[ir->rhs]);
            printf("    mov %s, rax\n", regs[ir->lhs]);
            break;
        // case '%':
        //     printf("    mov rax, %s\n", regs[ir->lhs]);
        //     printf("    cqo\n");
        //     printf("    div %s\n", regs[ir->rhs]);
        //     printf("    mov %s, rdi\n", regs[ir->lhs]);
        //     break;
        // case ND_EQ:
        //     printf("    cmp rdi, rax\n");
        //     printf("    sete al\n");
        //     printf("    movzb rax, al\n");
        //     break;
        // case ND_NE:
        //     printf("    cmp rdi, rax\n");
        //     printf("    setne al\n");
        //     printf("    movzb rax, al\n"); //al代入して上位56ビットは0クリア
        //     break;
        // case ND_AND:
        //     printf("    and rdi, rax\n");
        //     printf("    mov rax, rdi\n");
        //     break;
        // case ND_OR:
        //     printf("    or rdi, rax\n");
        //     printf("    mov rax, rdi\n");
        //     break;
        case IR_NOP:
            break;
        default:
            assert(0 && "unknown operator");
        }
    }
}
