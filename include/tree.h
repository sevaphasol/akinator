#ifndef TREE_H__
#define TREE_H__

#include <stdio.h>

/* Provided for reducing code size of checks in functions */
#define DO_IF(condition, action)                       \
if (condition)                                         \
{                                                      \
    fprintf(stderr, "%s in %s:%d:%s\n",                \
    #action, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    action;                                            \
}                                                      \

const size_t MinNumOfNodes = 256;
const size_t BufSize       = 100;

typedef int NodeData_t;

typedef enum TreeReturnCode
{
    TREE_SUCCESS,
    TREE_NULL_ARG_PTR_ERROR,
} TreeReturnCode;

typedef struct Node
{
    NodeData_t  data;
    Node*       left;
    Node*       right;
} Node_t;

typedef struct Tree
{
    size_t  size;
    Node_t* root;
    Node_t* memory;
    int     free_place;
} Tree_t;

TreeReturnCode TreeCtor       (Tree_t** ret_tree_ptr, size_t n_nodes, Node_t* root);
TreeReturnCode NodeCtor       (Node_t** ret_node_ptr);
TreeReturnCode JoinNode       (Node_t*  node, Node_t* left, Node_t* right);
TreeReturnCode PrintTextTree  (Node_t*  node);
TreeReturnCode PrintGraphTree (Node_t*  node, int file_number);

#endif // TREE_H__
