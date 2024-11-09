#ifndef NODE_ALLOCATOR_H__
#define NODE_ALLOCATOR_H__

#include "akinator.h"

//------------------------------------------------//

/* Provided for reducing code size of checks in functions */
#define VERIFY(condition, action)                                         \
if (condition)                                                            \
{                                                                         \
    fprintf(stderr, "%s in %s:%d:%s\n",                                   \
                    #action, __FILE__, __LINE__, __PRETTY_FUNCTION__);    \
    action;                                                               \
}                                                                         \

/* Acts like an usual assert, but does exit(EXIT_FAILURE) instead of abort()
   Provided for saving info in currently writing files when programm stops. */
#define ASSERT(condition)                                                 \
if (!condition)                                                           \
{                                                                         \
    fprintf(stderr, "%s:%d: %s: Assertion `%s' failed.\n",                \
                    __FILE__, __LINE__, __PRETTY_FUNCTION__, #condition); \
    exit(EXIT_FAILURE);                                                   \
}                                                                         \

//------------------------------------------------//

enum NodeAllocatorStatus
{
    NODE_ALLOCATOR_SUCCESS = 0,
    NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR,
    NODE_ALLOCATORE_STD_CALLOC_ERROR,
    NODE_ALLOCATOR_INVALID_NEW_NODE_ERROR,
    NODE_ALLOCATOR_BIG_ARRAY_REALLOC_ERROR,
    NODE_ALLOCATOR_ARRAYS_CALLOC_ERROR,
};

//------------------------------------------------//

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

typedef struct Allocator
{
    size_t   n_arrays;
    size_t   n_nodes_in_array;
    Node_t** big_array;
    Node_t*  answers;
    int      free_place;
} Allocator_t;

//------------------------------------------------//

NodeAllocatorStatus AllocatorCtor (Allocator_t* allocator,
                                   size_t n_arrays,
                                   size_t n_nodes_in_array);

NodeAllocatorStatus AllocatorDtor(Allocator_t* allocator);

NodeAllocatorStatus NodeCtor      (Allocator_t* allocator, Node_t** node);

//------------------------------------------------//

#endif // NODE_ALLOCATOR_H__
