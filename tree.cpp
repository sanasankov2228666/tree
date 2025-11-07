#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

int main(void)
{
    tree data = {};
    data.html_out = fopen("dump_classic.html", "w");

    tree_insert(&data, 5);
    tree_dump(&data, "1");

    tree_insert(&data, 10);
    tree_dump(&data, "2");

    tree_insert(&data, 3);
    tree_dump(&data, "3");

    tree_insert(&data, 1);
    tree_dump(&data, "4");

    tree_insert(&data, 4);
    tree_dump(&data, "5");

    tree_insert(&data, 3);
    tree_dump(&data, "6");


    preorder_print( get_root(&data) );
    printf("\n");

    inorder_print( get_root(&data) );
    printf("\n");

    postorder_print( get_root(&data) );
    printf("\n");

    deleter( get_root(&data) );

    return 0;
}

node_t* node_creator(tree* data, tree_t val)
{
    node_t* node = (node_t*) calloc (1, sizeof(node_t));
    if (node == NULL)
    {
        printf("node = NULL\n");
        return NULL;
    }

    node->val = val;

    return node;
}

err_t tree_insert(tree* data, tree_t val)
{   
    if (get_root(data) == NULL)
    {
        data->root = node_creator(data, val);
        return SUCCESS;
    }

    node_t* current_node = get_root(data);
    node_t* parent = NULL;

    while(current_node != NULL)
    {
        parent = current_node;
        
        if (val > current_node->val)
        {
            current_node = current_node->right;
        }

        else
        {
            current_node = current_node->left;
        }
    }

    current_node = node_creator(data, val);
    if (current_node == NULL) return ERROR;

    if (val > parent->val)
    {
        parent->right = current_node;
    }

    else
    {
       parent->left = current_node; 
    }

    return SUCCESS;
}

void preorder_print(node_t* root)
{
    if (root == NULL) return;

    printf("(%d ", root->val);

    preorder_print(root->left);
    preorder_print(root->right);

    printf(")");
}

void inorder_print(node_t* root)
{
    if (root == NULL) return;

    inorder_print(root->left);

    printf("(%d ", root->val);

    inorder_print(root->right);

    printf(")");
}

void postorder_print(node_t* root)
{
    if (root == NULL) return;
    
    postorder_print(root->left);
    postorder_print(root->right);
    
    printf("(%d ", root->val);

    printf(")");
}

void deleter(node_t* root)
{
    if (root == NULL) return;

    node_t* node_left = root->left;
    node_t* node_right = root->right;

    free(root);

    deleter(node_left);
    deleter(node_right);
}

void dot_node_gen(node_t* root, FILE* dot_file, size_t* number)
{
    if (root == NULL) return;

    fprintf(dot_file, "\tnode%zu [label = \"{data = %d| {<left> left = %p|<right> right = %p} }\"];\n",
            *number, root->val, root->left, root->right);
    
    size_t current_number = (*number)++;
    
    if (root->left != NULL)
    {
        dot_node_gen(root->left, dot_file, number);
    }
    
    if (root->right != NULL)
    {
        dot_node_gen(root->right, dot_file, number);
    }
}

void dot_arrw_gen(node_t* root, FILE* dot_file, size_t* number, size_t parent_number)
{
    if (root == NULL) return;

    size_t current_number = (*number)++;
    
    if (root->left != NULL)
    {
        fprintf(dot_file, "\tnode%zu:<left> -> node%zu;\n", 
                current_number, *number);
        dot_arrw_gen(root->left, dot_file, number, current_number);
    }
    
    if (root->right != NULL)
    {
        fprintf(dot_file, "\tnode%zu:<right> -> node%zu;\n", 
                current_number, *number);
        dot_arrw_gen(root->right, dot_file, number, current_number);
    }
}

err_t dump_generator(tree* data, const char* filename, const char* reason)
{
    char dot_filename[200] = {};
    snprintf(dot_filename, sizeof(dot_filename), "dot/%s.dot", filename);
    
    FILE* dot_file = fopen(dot_filename, "w");
    if (!dot_file)
    {
        printf("Error creating DOT file\n");
        return ERROR;
    }
    
    fprintf(dot_file, "digraph Tree\n{\n");
    fprintf(dot_file, "\tnodesep=0.8;\n");
    fprintf(dot_file, "\tranksep=1;\n");
    fprintf(dot_file, "\tnode [shape=record,style=\"rounded,filled\", fillcolor=lightblue];\n");
    fprintf(dot_file, "\tedge [arrowsize=0.8];\n");
    fprintf(dot_file, "\tsplines=ortho;");
    fprintf(dot_file, "\tfontsize = 24;\n");
    fprintf(dot_file, "\tlabel=\"%s\";\n", reason);
    fprintf(dot_file, "\tlabelloc=t;\n\n");

    size_t node_number = 0;
    if (data->root != NULL)
    {
        dot_node_gen(data->root, dot_file, &node_number);
    }

    size_t arrow_number = 0;
    if (data->root != NULL)
    {
        dot_arrw_gen(data->root, dot_file, &arrow_number, 0);
    }

    fprintf(dot_file, "\n}");
    fclose(dot_file);
    
    char command[400];
    snprintf(command, sizeof(command), "dot -Tpng dot/%s.dot -o png/%s.png", filename, filename);
    
    int result = system(command);
    if (result != 0)
    {
        printf("ERROR generating PNG\n");
        return ERROR;
    }
    
    printf("PNG was generated: %s.png\n", filename);

    html_generator(filename, data);

    return SUCCESS;
}

err_t html_generator(const char* filename, tree* data)
{
    fprintf(data->html_out, "<figure>\n");
    fprintf(data->html_out, "\t<img src=\"png/%s.png\"/>\n", filename);
    fprintf(data->html_out, "</figure>\n");
    fprintf(data->html_out, "<br><br><br><br><br><br>\n");

    return 0;
}

void tree_dump(tree* data, const char* reason)
{
    static int dump_count = 0;
    char filename[40] = {};
    snprintf(filename, sizeof(filename), "tree_dump_%d", dump_count++);
    
    if (dump_generator(data, filename, reason) ) printf("PNG generate failed\n");
}

node_t* get_root(tree* data)
{
    return data->root;
}