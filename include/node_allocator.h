#ifndef NODE_ALLOCATOR_H__
#define NODE_ALLOCATOR_H__

//————————————————————————————————————————————————//

#include "akinator.h"

//————————————————————————————————————————————————//

enum NodeAllocatorStatus
{
    NODE_ALLOCATOR_SUCCESS = 0,
    NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR,
    NODE_ALLOCATORE_STD_CALLOC_ERROR,
    NODE_ALLOCATOR_INVALID_NEW_NODE_ERROR,
    NODE_ALLOCATOR_BIG_ARRAY_REALLOC_ERROR,
    NODE_ALLOCATOR_ARRAYS_CALLOC_ERROR,
};

//————————————————————————————————————————————————//

typedef struct NodeData
{
    bool  is_question;
    char* str;
} NodeData_t;

typedef struct Node
{
    int        level;

    NodeData_t data;

    Node*      left;
    Node*      right;
} Node_t;

//------------------------------------------------//

typedef struct Allocator
{
    size_t   n_arrays;
    size_t   n_nodes_in_array;
    Node_t** big_array;
    int      free_place;

    size_t   n_answers;
    Node_t** answers;
} Allocator_t;

//————————————————————————————————————————————————//

NodeAllocatorStatus AllocatorCtor (Allocator_t* allocator,
                                   size_t n_arrays,
                                   size_t n_nodes_in_array);

NodeAllocatorStatus AllocatorDtor (Allocator_t* allocator);

//------------------------------------------------//

NodeAllocatorStatus NodeCtor      (Allocator_t* allocator, Node_t** node);

//————————————————————————————————————————————————//

#endif // NODE_ALLOCATOR_H__
