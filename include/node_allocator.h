#ifndef NODE_ALLOCATOR_H__
#define NODE_ALLOCATOR_H__

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
    size_t     level;

    NodeData_t data;

    Node*      parent;
    bool       left_to_parent;

    Node*      left;
    Node*      right;
} Node_t;

typedef struct Answer
{
    Node_t* node;
    bool    is_positive;
} Answer_t;

//------------------------------------------------//

typedef struct NodeAllocator
{
    size_t   n_arrays;
    size_t   n_nodes_in_array;
    Node_t** big_array;
    int      free_place;

    size_t   n_answers;
    Node_t** answers;

    size_t   tree_level;
} NodeAllocator_t;

//————————————————————————————————————————————————//

NodeAllocatorStatus NodeAllocatorCtor (NodeAllocator_t* node_allocator,
                                       size_t           n_arrays,
                                       size_t           n_nodes_in_array);

NodeAllocatorStatus NodeAllocatorDtor (NodeAllocator_t* allocator);

//------------------------------------------------//

NodeAllocatorStatus NodeCtor          (NodeAllocator_t* node_allocator,
                                       Node_t**         node,
                                       Node_t*          parent,
                                       bool             left_to_parent);

//————————————————————————————————————————————————//

#endif // NODE_ALLOCATOR_H__
