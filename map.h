#ifndef MAP_H
#define MAP_H

#include "rb_tree.h"

typedef rb_tree Map;

//Создание словаря с функцией сравнения ключей 
Map* map_create(int (*cmp)(const void*, const void*));

//Вставка или обновление: 0 – новый, 1 – обновлён 
int map_insert(Map* m, void* key, void* value);

//Получить значение по ключу (NULL если нет)
void* map_find(Map* m, void* key);

//Удалить ключ: 1 – успех, 0 – ключ не найден 
int map_erase(Map* m, void* key);

//Обход в порядке возрастания ключей 
void map_foreach(Map* m, void (*func)(void* key, void* value));

//Удалить все элементы
void map_destroy(Map* m);

//Количество элементов 
static inline int map_size(Map* m) {
    return ((rb_tree*)m)->size;
}


#endif
