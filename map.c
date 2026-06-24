#include "map.h"
#include <stdlib.h>

Map* map_create(int (*cmp)(const void*, const void*)) {
    return rb_create(cmp);
}

int map_insert(Map* m, void* key, void* value) {
    return rb_insert(m, key, value);
}

void* map_find(Map* m, void* key) {
    return rb_find(m, key);
}

int map_erase(Map* m, void* key) {
    return rb_erase(m, key);
}

void map_foreach(Map* m, void (*func)(void*, void*)) {
    rb_foreach(m, func);
}

void map_destroy(Map* m) {
    rb_destroy(m);
}