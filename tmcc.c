#include "tmcc.h"
#include "util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vector *tokens;
// int pos = 0;
// Vector *code;
// Map *var_tab;
// int var_cnt = 0;

int main(int argc, char **argv){
    // bool ast_flg = false;
    // tokens = new_vector();
    // code = new_vector();
    // var_tab = new_map();

    if(argc != 2){
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    // if(argc >= 2 && strcmp(argv[1], "-test") == 0){
    //     runtest();
    //     return 0;
    // }

    if(!strcmp(argv[1], "-test")){
        runtest();
        return 0;
    }

    // if(argc >= 2 && strcmp(argv[1], "-ast") == 0){
    //     ast_flg = true;
    //     argc--;
    //     argv++;
    // }

    // if(argc != 2){
    //     fprintf(stderr, "引数の数が正しくありません");
    //     fprintf(stderr, "%s: [-ast] program", argv[0]);
    //     fprintf(stderr, "%s: [-test]", argv[0]);
    //     return EXIT_FAILURE;
    // }

    //トークナイズしてパースする
    Vector *tokens = tokenize(argv[1]);
    Node* node = parse(tokens);

    Vector *irv = gen_ir(node);
    alloc_regs(irv);
 
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    gen_x86(irv);
    return 0;

    // if(ast_flg == true){
    //     printf("graph name {\n");
    //     for(int i=0;i<code->len;i++)
    //         p_tree((Node *)code->data[i]);
    //     printf("}\n");
    //     return EXIT_SUCCESS;
    // }

    // //プロローグ
    // //変数26分の領域を確保する
    // printf("    push rbp\n");
    // printf("    mov rbp, rsp\n");
    // printf("    sub rsp, %d\n", var_cnt * 8);

    // //先頭の式から順にコード生成
    // for(int i=0; i<code->len; i++){
    //     gen((Node *)code->data[i]);

    //     //式の評価結果としてスタックに一つの値が残っている
    //     //はずなので、スタックが溢れないようにポップしておく
    //     printf("    pop rax\n");
    // }

    // //エピローグ
    // //最後の結果がRAXに残っているのでそれが返り値になる
    // printf("    mov rsp, rbp\n");
    // printf("    pop rbp\n");
    // printf("    ret\n");
    // return EXIT_SUCCESS;
}


