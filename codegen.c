#include "tmcc.h"
#include "util.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static char *gen_label(){
    static int n;
    char buf[10];
    sprintf(buf, ".L%d", n++);
    return strdup(buf);
}

void gen_x86(Vector *irv){
    char *ret = gen_label();

    //rbpはスタック内のアクセスとして使う
    printf("    push rbp\n");       //base pointer
    printf("    mov rbp, rsp\n");   //stack pointer
    
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
            printf("    jmp %s\n", ret);
            break;
        case IR_ALLOCA:
            //右辺分のメモリを空ける
            if(ir->rhs)
                printf("    sub rsp, %d\n", ir->rhs);
            printf("    mov %s, rsp\n", regs[ir->lhs]);
            break;
        case IR_LOAD:
            //アドレスアクセス。右辺レジスタが保持しているアドレスの情報を左辺レジスタに移動する
            //つまりメモリからレジスタへコピー
            printf("    mov %s, [%s]\n", regs[ir->lhs], regs[ir->rhs]);
            break;
        case IR_STORE:
            //レジスタからメモリへコピー
            printf("    mov [%s], %s\n", regs[ir->lhs], regs[ir->rhs]);
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
        case '%':
            printf("    mov rax, %s\n", regs[ir->lhs]);
            printf("    cqo\n");
            printf("    div %s\n", regs[ir->rhs]);
            //除算の余りはrdxに格納される
            printf("    mov %s, rdx\n", regs[ir->lhs]);
            break;
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

    //ここの意味がよくわからない
    //ベースポインタの上から２つスタックポインタに代入して残りのベースポインタを出力している
    printf("    %s:\n", ret);
    printf("    mov rsp, rbp\n");
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
}
