#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "rb_tree.h"
#include "map.h"
#include "tests.h"

// Общий компаратор для целых 
static int cmp_int(const void* a, const void* b) {
    int ia = *(const int*)a, ib = *(const int*)b;
    return (ia > ib) - (ia < ib);
}

// Вспомогательные переменные для проверки порядка 
static int prev_key;
static int ordered_flag;

static void check_order(void* k, void* val) {
    (void)val;                     
    int cur = *(int*)k;
    if (cur <= prev_key)
        ordered_flag = 0;
    prev_key = cur;
}

void run_automatic_tests(void) {
    printf("Автоматические тесты\n");

    Map* m = map_create(cmp_int);
    int keys[] = { 50, 30, 70, 20, 40, 60, 80 };
    int vals[] = { 50, 30, 70, 20, 40, 60, 80 };

    for (int i = 0; i < 7; i++)
        map_insert(m, &keys[i], &vals[i]);
    assert(map_size(m) == 7);

    int* v = map_find(m, &keys[4]);
    assert(v != NULL && *v == 40);
    assert(map_find(m, &(int){100}) == NULL);

    int new_val = 999;
    assert(map_insert(m, &keys[0], &new_val) == 1);
    v = map_find(m, &keys[0]);
    assert(v && *v == 999);

    assert(map_erase(m, &keys[2]) == 1);
    assert(map_size(m) == 6);
    assert(map_find(m, &keys[2]) == NULL);
    assert(map_erase(m, &keys[2]) == 0);

    // Проверка порядка обхода
    ordered_flag = 1;
    prev_key = -1;
    map_foreach(m, check_order);
    assert(ordered_flag);

    map_destroy(m);
    printf("Все проверки пройдены.\n\n");
}

// Демонстрационные примеры 

// 1. Прямая работа с красно-чёрным деревом 
static void print_tree_node(rb_node* node, int depth) {
    if (!node) return;
    print_tree_node(node->right, depth + 1);
    for (int i = 0; i < depth; i++) printf("    ");
    printf("%d(%s)\n", *(int*)node->key, node->color == RED ? "R" : "B");
    print_tree_node(node->left, depth + 1);
}

static void demo_rb_tree(void) {
    printf("Демонстрация красно-чёрного дерева\n");
    rb_tree* t = rb_create(cmp_int);
    int data[] = { 13, 8, 17, 1, 11, 6, 15, 25, 22, 27 };
    for (int i = 0; i < 10; i++)
        rb_insert(t, &data[i], NULL);

    printf("Структура дерева (повёрнуто на 90°):\n");
    print_tree_node(t->root, 0);
    printf("Размер: %d\n\n", t->size);
    rb_destroy(t);
}

//2. Телефонный справочник (строки)
static int cmp_str(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

static void print_contact(void* key, void* value) {
    printf("  %-15s : %s\n", (char*)key, (char*)value);
}

static void demo_phonebook(void) {
    printf("Телефонный справочник\n");
    Map* book = map_create(cmp_str);

    char* names[] = { "Bob", "Charlie", "Alice", "Diana" };
    char* phones[] = { "+7-111-11-11", "+7-222-22-22", "+7-333-33-33", "+7-444-44-44" };

    for (int i = 0; i < 4; i++)
        map_insert(book, names[i], phones[i]);

    printf("Все контакты (отсортированы по имени):\n");
    map_foreach(book, print_contact);

    printf("\nПоиск номера Alice: %s\n", (char*)map_find(book, "Alice"));
    printf("Удаляем Bob...\n");
    map_erase(book, "Bob");
    printf("После удаления:\n");
    map_foreach(book, print_contact);

    map_destroy(book);
    putchar('\n');
}

//3. Подсчёт частоты слов
static void print_wc(void* k, void* v) {
    printf("  %-10s : %d\n", (char*)k, *(int*)v);
}

static void demo_word_count(void) {
    printf("Подсчёт частоты слов\n");
    Map* counter = map_create(cmp_str);
    char text[] = "apple banana apple orange banana apple orange orange grape";
    char* word = strtok(text, " ");

    while (word) {
        int* cnt = map_find(counter, word);
        if (cnt) {
            (*cnt)++;
        }
        else {
            int* p = malloc(sizeof(int));
            *p = 1;
            char* key = strdup(word);   // надёжная копия ключа
            map_insert(counter, key, p);
        }
        word = strtok(NULL, " ");
    }

    printf("Слово -> частота:\n");
    map_foreach(counter, print_wc);

    map_destroy(counter);
}

//Сборка всех демонстраций
void run_demo_tests(void) {
    demo_rb_tree();
    demo_phonebook();
    demo_word_count();
}