#include "tmcc.h"

static Vector *code;

static IR *add(int op, int lhs, int rhs){
    IR *ir = malloc(sizeof(IR));
    ir->op = op;
    ir->lhs = lhs;
    ir->rhs = rhs;
    vec_push(code, ir);
    return ir;
}

static int gen_expr(Node *node){
    static int regno;

    if(node->ty == ND_NUM){
        int reg = regno++;
        add(IR_IMM, reg, node->val);
        return reg;
    }

    assert(strchr("+-*/\%", node->ty));

    int lhs = gen_expr(node->lhs);
    int rhs = gen_expr(node->rhs);

    add(node->ty, lhs, rhs);
    add(IR_KILL, rhs, 0);
    return lhs;
}

//generate statement
static void gen_stmt(Node *node){
    if(node->ty == ND_RETURN){
        int reg = gen_expr(node->expr);
        add(IR_RETURN, reg, 0);
        add(IR_KILL, reg, 0);
        return;
    }

    if(node->ty == ND_EXPR_STMT){
        int reg = gen_expr(node->expr);
        add(IR_KILL, reg, 0);
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
    gen_stmt(node);
    return code;
}