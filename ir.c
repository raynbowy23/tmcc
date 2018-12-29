#include "tmcc.h"

static Vector *code;
static int regno;
static int basereg;

static Map *vars;
static int bpoff;

static IR *add(int op, int lhs, int rhs){
    IR *ir = malloc(sizeof(IR));
    ir->op = op;
    ir->lhs = lhs;
    ir->rhs = rhs;
    vec_push(code, ir);
    return ir;
}

//left value
//与えられたノードが変数を指しているときに、その変数のアドレスを計算して、
//それをスタックにプッシュします。それ以外の場合にはエラーを表示します。
static int gen_lval(Node *node){
    if(node->ty != ND_IDENT)
        error("not an lvalue");

    //存在していない識別子ならば新たに加える
    if(!map_exists(vars, node->name)){
        map_put(vars, node->name, (void *)(intptr_t)bpoff);
        bpoff += 8;
    }

    int r1 = regno++;
    int off = (intptr_t)map_get(vars, node->name);
    add(IR_MOV, r1, basereg);

    int r2 = regno++;
    add(IR_IMM, r2, off);
    add('+', r1, r2);
    add(IR_KILL, r2, -1);
    return r1;
}

static int gen_expr(Node *node){

    if(node->ty == ND_NUM){
        int reg = regno++;
        add(IR_IMM, reg, node->val);
        return reg;
    }

    if(node->ty == ND_IDENT){
        int reg = gen_lval(node);
        add(IR_LOAD, reg, reg);
        return reg;
    }

    if(node->ty == '='){
        int rhs = gen_expr(node->rhs);
        int lhs = gen_lval(node->lhs);
        add(IR_STORE, lhs, rhs);
        add(IR_KILL, rhs, -1);
        return lhs;
    }

    assert(strchr("+-*/\%", node->ty));

    int lhs = gen_expr(node->lhs);
    int rhs = gen_expr(node->rhs);

    add(node->ty, lhs, rhs);
    add(IR_KILL, rhs, -1);
    return lhs;
}

//generate statement
static void gen_stmt(Node *node){
    if(node->ty == ND_RETURN){
        int reg = gen_expr(node->expr);
        add(IR_RETURN, reg, -1);
        add(IR_KILL, reg, -1);
        return;
    }

    if(node->ty == ND_EXPR_STMT){
        int reg = gen_expr(node->expr);
        add(IR_KILL, reg, -1);
        return;
    }

    if(node->ty == ND_COMP_STMT){
        for(int i=0;i<node->stmts->len;i++)
            gen_stmt(node->stmts->data[i]);
        return;
    }

    error("unkown node: %s", node->ty);
}

Vector *gen_ir(Node *node){
    assert(node->ty == ND_COMP_STMT);

    code = new_vector();
    regno = 1;
    basereg = 0;
    vars = new_map();
    bpoff = 0;

    IR *alloca = add(IR_ALLOCA, basereg, -1);
    gen_stmt(node);
    alloca->rhs = bpoff;
    add(IR_KILL, basereg, -1);
    return code;
}