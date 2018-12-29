#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>

//util
typedef struct{
    void **data;
    int capacity;
    int len;
}Vector;

Vector *new_vector();
void vec_push(Vector *vec, void *elem);

typedef struct{
    Vector *keys;
    Vector *vals;
}Map;

Map *new_map(void);
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);
bool map_exists(Map *map, char *key);

int *intdup(int);

_Noreturn void error(char *, ...);

//util_test
void runtest();

#endif