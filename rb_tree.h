#ifndef RB_TREE_H
#define RB_TREE_H

typedef enum {RED, BLACK}Color;
//Узел дерева
typedef struct rb_node{
	void* key;
	void* value;
	Color color;
	struct rb_node *left, *right, *parent;
}rb_node;

typedef struct  {
	rb_node* root;
	int(*cmp)(const void*, const void*);
	int size;
}rb_tree;

//создание дерева
rb_tree* rb_create(int(*cmp)(const void*, const void*));

//Вставка / обновление. Возвращает 0, если ключ новый, 1 – если обновлён 
int rb_insert(rb_tree* t, void* key, void* value);

//Поиск значения по ключу. Возвращает указатель на value или NULL
void* rb_find(rb_tree* t, void* key);

//Удаление ключа. Возвращает 1, если элемент был удалён, иначе 0
int rb_erase(rb_tree* t, void* key);

//Обход дерева в порядке возрастания ключей
void rb_foreach(rb_tree* t, void(*func)(void* key, void* value));

//Удаление всех узлов 
void rb_destroy(rb_tree* t);
#endif