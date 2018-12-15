//トークンの型を表す値
enum{
    TK_NUM = 256, //整数トークン
    TK_IDENT,     //識別子
    TK_EOF,       //入力の終わりを表すトークン
};

//トークンの型
typedef struct{
    int ty;      //トークンの型
    int val;     //tyがtk_numの場合、その数値
    char *input; //トークン文字列（エラーメッセージ用）
}Token;

enum{
    ND_NUM = 256,   //整数のノードの型
    ND_IDENT,       //識別子のノードの型
};

typedef struct Node{
    int ty;             //演算子かnd_num
    struct Node *lhs;   //左辺
    struct Node *rhs;   //右辺
    int val;            //tyがnd_numの場合のみ使う
    char name;          //tyがnd_identの場合のみ使う
}Node;

typedef struct{
    void **data;
    int capacity;
    int len;
}Vector;

typedef struct{
    Vector *keys;
    Vector *vals;
}Map;

extern Token tokens[];
extern Node *code[];

void gen();
void program();
__attribute__((noreturn)) void error(char *fmt, ...);

void runtest();
Vector *new_vector();
void vec_push(Vector *vec, void *elem);
Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);
