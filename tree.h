#ifndef TREE_H_
#define TREE_H_

#include <stdio.h>

typedef int err_t;

typedef int tree_t;

enum
{
    SUCCESS = 0,
    ERROR   = 1
};

struct node_t
{
    tree_t val  = 0;
    node_t* right = NULL;
    node_t* left = NULL;
};

struct tree
{
    node_t* root    = NULL;
    size_t error    = 0;
    FILE*  html_out = NULL;
};


node_t* node_creator(tree* data, tree_t val);

err_t tree_insert(tree* data, tree_t val);

void deleter(node_t* root);

node_t* get_root(tree* data);


void preorder_print(node_t* head);

void inorder_print(node_t* root);

void postorder_print(node_t* root);


err_t dump_generator(tree* data, const char* filename, const char* reason);

void dot_node_gen(node_t* root, FILE* dot_file, size_t* number);

void dot_arrw_gen(node_t* root, FILE* dot_file, size_t* number, size_t parent_number);

void tree_dump(tree* data, const char* reason);

err_t html_generator(const char* filename, tree* data);

#endif