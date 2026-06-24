#include"rb_tree.h"
#include<stdlib.h>

static rb_node* new_node(void* key, void* value, rb_node* parent) {
	rb_node* n = malloc(sizeof(rb_node));
	n->key = key;
	n->value = value;
	n->color = RED;//всегда красный, чтобы не нарушить "чёрную высоту". В случае необходимости исправляется поворотами и перекрашиваниями
	n->left = n->right = NULL;
	n->parent = parent;
	return n;
}

rb_tree* rb_create(int (*cmp)(const void*, const void*)) {
	rb_tree* t = malloc(sizeof(rb_tree));
	t->root = NULL;
	t->cmp = cmp;
	t->size = 0;
	return t;
}

//повороты
static void rotate_left(rb_tree* t, rb_node* x) {
    rb_node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) t->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rotate_right(rb_tree* t, rb_node* y) {
    rb_node* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) t->root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}

//Балансировка после вставки: повороты и перекрасы
static void insert_fixup(rb_tree* t, rb_node* z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            rb_node* y = z->parent->parent->right; //"дядя"
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotate_left(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(t, z->parent->parent);
            }
        }
        else {
            rb_node* y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotate_right(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(t, z->parent->parent);
            }
        }
    }
    t->root->color = BLACK;
}

int rb_insert(rb_tree* t, void* key, void* value) {
    rb_node* parent = NULL, *cur = t->root;
    int cmp_res = 0;
    while (cur) {
        parent = cur;
        cmp_res = t->cmp(key, cur->key);
        if (cmp_res == 0) {
            cur->value = value;//обновление значения
            return 1;
        }
        cur = (cmp_res < 0) ? cur->left : cur->right;

    }
    rb_node* node = new_node(key, value, parent);
    if (!parent)t->root = node;
    else if (cmp_res < 0)parent->left = node;
    else parent->right = node;
    t->size++;
    insert_fixup(t, node);
    return 0;

}
//поиск

void* rb_find(rb_tree* t, void* key) {
    rb_node* cur = t->root;
    while (cur) {
        int c = t->cmp(key, cur->key);
        if (c == 0)return cur->value;
        cur = (c < 0) ? cur->left : cur->right;
    }
    return NULL;
}

//Вспомогательные функции удаления 
static void transplant(rb_tree* t, rb_node* u, rb_node* v) {
    if (!u->parent) t->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}

static rb_node* tree_minimum(rb_node* x) {
    while (x->left) x = x->left;
    return x;
}

static void delete_fixup(rb_tree* t, rb_node* x) {
    while (x != t->root && (x == NULL || x->color == BLACK)) {
        if (x == x->parent->left) {
            rb_node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(t, x->parent);
                w = x->parent->right;
            }
            if ((!w->left || w->left->color == BLACK) &&
                (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (!w->right || w->right->color == BLACK) {
                    if (w->left) w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(t, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                rotate_left(t, x->parent);
                x = t->root;
            }
        }
        else {
            rb_node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_right(t, x->parent);
                w = x->parent->left;
            }
            if ((!w->right || w->right->color == BLACK) &&
                (!w->left || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (!w->left || w->left->color == BLACK) {
                    if (w->right) w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(t, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                rotate_right(t, x->parent);
                x = t->root;
            }
        }
    }
    if (x) x->color = BLACK;
}

int rb_erase(rb_tree* t, void* key) {
    rb_node* z = t->root;
    while (z) {
        int c = t->cmp(key, z->key);
        if (c == 0) break;
        z = (c < 0) ? z->left : z->right;
    }
    if (!z) return 0;

    rb_node* y = z;
    Color y_orig_color = y->color;
    rb_node* x;

    if (!z->left) {
        x = z->right;
        transplant(t, z, z->right);
    }
    else if (!z->right) {
        x = z->left;
        transplant(t, z, z->left);
    }
    else {
        y = tree_minimum(z->right);
        y_orig_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        }
        else {
            transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    free(z);
    t->size--;

    if (y_orig_color == BLACK)
        delete_fixup(t, x);

    return 1;
}

//Обход 
static void inorder_do(rb_node* node, void (*func)(void*, void*)) {
    if (!node) return;
    inorder_do(node->left, func);
    func(node->key, node->value);
    inorder_do(node->right, func);
}

void rb_foreach(rb_tree* t, void (*func)(void*, void*)) {
    inorder_do(t->root, func);
}

//Очистка 
static void free_nodes(rb_node* node) {
    if (!node) return;
    free_nodes(node->left);
    free_nodes(node->right);
    free(node);
}

void rb_destroy(rb_tree* t) {
    free_nodes(t->root);
    free(t);
}