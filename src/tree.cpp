#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

static TreeReturnCode MakeDotNode(Node_t* node, FILE* file, int* node_number);

//------------------------------------------------//

TreeReturnCode NodeCtor(Node_t** ret_node_ptr)
{
    DO_IF(!ret_node_ptr, return TREE_NULL_ARG_PTR_ERROR);

    *ret_node_ptr = (Node*) calloc(1, sizeof(Node));

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode JoinTopNode(Node_t* node, Node_t* left, Node_t* right)
{
    DO_IF(!node, return TREE_NULL_ARG_PTR_ERROR);

    node->left  = left;
    node->right = right;

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode JoinLowNode(Node_t* node, Node_t* parent)
{
    DO_IF(!node || !parent, return TREE_NULL_ARG_PTR_ERROR);

    if (node->data)

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode JoinNodeByValue(Node_t* parent, NodeData_t value)
{
    DO_IF(!parent, return TREE_NULL_ARG_PTR_ERROR);

    if (value <= parent->data)
    {
        next_node = parent->left;
    }
    else
    {
        next_node = cur_node->right;
    }

    if (next_node)
    {
        cur_node = next_node;
        continue;
    }

    NodeCtor(&next_node);
    cur_node->
    cur_node->left->data = value;
}

//------------------------------------------------//

TreeReturnCode TreeCtor(Tree_t** ret_tree_ptr, size_t n_nodes)
{
    DO_IF(!ret_tree_ptr, return TREE_NULL_ARG_PTR_ERROR);

    if (n_nodes < MinNumOfNodes)
    {
        n_nodes = MinNumOfNodes;
    }

    Tree_t* tree     = (Tree_t*) calloc(sizeof(ret_tree_ptr) + tree->size * sizeof(Node_t), sizeof(char));
    tree->size       = n_nodes;
    tree->memory     = (Node_t*) tree + 1;
    tree->free_place = 0;

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode PrintTextTree (Node_t* node)
{
    DO_IF(!node, return TREE_NULL_ARG_PTR_ERROR);

    putchar('(');
    if (node->left) PrintTextTree(node->left);
    printf("%d", node->data);
    if (node->right) PrintTextTree(node->right);
    putchar(')');

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode PrintGraphTree(Node_t* node, int file_number)
{
    DO_IF(!node, return TREE_NULL_ARG_PTR_ERROR);

    char dot_file_name[BufSize] = {};
    snprintf(dot_file_name, BufSize, "logs/dot_files/%03d.dot", file_number);
    FILE* dot_file = fopen(dot_file_name, "w");

    fputs("digraph G{\n"
          "rankdir=HR;\n"
          "node[color=\"red\",fontsize=14];\n"
          "edge[color=\"#00eeee80\",fontsize=12, penwidth=1];\n",
          dot_file);

    int node_number = 1;

    MakeDotNode(node, dot_file, &node_number);

    fputs("}\n", dot_file);

    fclose(dot_file);

    char png_file_name[BufSize] = {};
    snprintf(png_file_name, BufSize, "logs/images/%03d.png", file_number);

    char command[4*BufSize] = {};
    snprintf(command, 4*BufSize, "touch %s; dot %s -Tpng -o %s", png_file_name, dot_file_name, png_file_name);

    system(command);

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode MakeDotNode(Node_t* node, FILE* file, int* node_number)
{
    DO_IF(!node, return TREE_NULL_ARG_PTR_ERROR);

    fprintf(file, "elem%d["
                  "shape=\"Mrecord\", "
                  "label=\"{ %d | data = %d | left = %p | right = %p }"
                  "\"];\n",
                  *node_number, *node_number , node->data, node->left, node->right);

    int head_node_number = *node_number;

    if (node->left)
    {
        int left_node_number = ++*node_number;

        fprintf(file, "elem%d->elem%d;", head_node_number, left_node_number);

        MakeDotNode(node->left, file, node_number);
    }

    if (node->right)
    {
        int right_node_number = ++*node_number;

        fprintf(file, "elem%d->elem%d;", head_node_number, right_node_number);

        MakeDotNode(node->right, file, node_number);
    }

    return TREE_SUCCESS;
}

//------------------------------------------------//
