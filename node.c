#include "tmcc.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// static int pos;

// Node *new_node(int op, Node *lhs, Node *rhs) {
//     Node *node = malloc(sizeof(Node));
//     node->ty = op;
//     node->lhs = lhs;
//     node->rhs = rhs;
//     return node;
// }

// Node *new_node_num(int val) {
//     Token *token = tokens->data[pos];
//     if(token->ty != TK_NUM)
//         error("number expected, but got %s", token->input);
//     pos++;

//     Node *node = malloc(sizeof(Node));
//     node->ty = ND_NUM;
//     node->val = token->val;
//     return node;
// }

// Node *new_node_ident(char *name) {
    // Node *node = malloc(sizeof(Node));
    // node->ty = ND_IDENT;
    // node->name = malloc(sizeof(char) * strlen(name));
    // strcpy(node->name, name);
    // return node;
// }
