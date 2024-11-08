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

const size_t MinNumOfNodes     = 256;
const size_t FileNameBufSize   = 64;
const size_t SysCommandBufSize = 278;
const size_t NodeDataStrSize   = 128;

typedef enum TreeReturnCode
{
    TREE_SUCCESS,
    TREE_NULL_ARG_PTR_ERROR,
} TreeReturnCode;

typedef struct NodeData
{
    bool  is_question;
    char* str;
} NodeData_t;

typedef struct Node
{
    NodeData_t  data;
    Node*       left;
    Node*       right;
} Node_t;

// typedef struct Tree
// {
//     size_t     size;
//     Node_t*    root;
//     DataBase_t db;
// } Tree_t;

Node_t*        NodeCtor();
TreeReturnCode PrintGraphTree (Node_t*  node, int file_number);
TreeReturnCode StartGame      (Node_t* node);

#endif // TREE_H__
