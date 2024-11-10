#include <stdlib.h>

//------------------------------------------------//

#include "akinator.h"
#include "node_allocator.h"
#include "custom_assert.h"

//————————————————————————————————————————————————//

static NodeAllocatorStatus BigArrayRealloc(NodeAllocator_t* node_allocator);
static NodeAllocatorStatus ArrayCalloc    (NodeAllocator_t* node_allocator);

//————————————————————————————————————————————————//

NodeAllocatorStatus NodeAllocatorCtor(NodeAllocator_t* node_allocator,
                                      size_t n_arrays,
                                      size_t n_nodes_in_array)
{
    VERIFY(!node_allocator,
           return NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    //------------------------------------------------//

    node_allocator->n_arrays         = n_arrays;

    node_allocator->big_array        = (Node_t**) calloc(n_arrays, sizeof(Node_t**));
    VERIFY(!node_allocator->big_array,
           return NODE_ALLOCATORE_STD_CALLOC_ERROR);

    //------------------------------------------------//

    node_allocator->n_nodes_in_array = n_nodes_in_array;

    for (int i = 0; i < n_arrays; i++)
    {
        node_allocator->big_array[i] = (Node_t*) calloc (n_nodes_in_array, sizeof(Node_t*));
        VERIFY(!node_allocator->big_array[i],
               return NODE_ALLOCATORE_STD_CALLOC_ERROR);
    }

    node_allocator->free_place = 0;

    //------------------------------------------------//

    node_allocator->answers = (Node_t**) calloc (n_nodes_in_array, sizeof(Node_t*));
    VERIFY(!node_allocator->answers,
           return NODE_ALLOCATORE_STD_CALLOC_ERROR);

    //------------------------------------------------//

    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus NodeAllocatorDtor(NodeAllocator_t* node_allocator)
{
    VERIFY(!node_allocator,
           return NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    //------------------------------------------------//

    for (int i = 0; i < node_allocator->n_arrays; i++)
    {
        free(node_allocator->big_array[i]);
    }

    free(node_allocator->big_array);

    node_allocator->free_place       = 0;
    node_allocator->n_arrays         = 0;
    node_allocator->n_nodes_in_array = 0;

    //------------------------------------------------//

    free(node_allocator->answers);

    node_allocator->n_answers = 0;

    //------------------------------------------------//

    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus NodeCtor(NodeAllocator* node_allocator, Node_t** new_node)
{
    VERIFY(!node_allocator,
            return NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    VERIFY(!new_node,
           return NODE_ALLOCATOR_INVALID_NEW_NODE_ERROR);

    //------------------------------------------------//

    if (node_allocator->free_place >= node_allocator->n_arrays * node_allocator->n_nodes_in_array)
    {
        VERIFY(BigArrayRealloc(node_allocator),
               return NODE_ALLOCATOR_BIG_ARRAY_REALLOC_ERROR);

        VERIFY(ArrayCalloc(node_allocator),
               return NODE_ALLOCATOR_ARRAYS_CALLOC_ERROR);
    }

    //------------------------------------------------//

    size_t cur_array      = node_allocator->free_place / node_allocator->n_nodes_in_array;
    size_t rel_free_place = node_allocator->free_place % node_allocator->n_nodes_in_array;

    *new_node = &node_allocator->big_array[cur_array][rel_free_place];

    node_allocator->free_place++;

    //------------------------------------------------//

    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus BigArrayRealloc(NodeAllocator_t* node_allocator)
{
    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus ArrayCalloc(NodeAllocator_t* node_allocator)
{
    return NODE_ALLOCATOR_SUCCESS;
}

//————————————————————————————————————————————————//
