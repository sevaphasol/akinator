#include <stdlib.h>

#include "akinator.h"
#include "node_allocator.h"
#include "custom_assert.h"

//------------------------------------------------//

static NodeAllocatorStatus BigArrayRealloc(Allocator_t* allocator);
static NodeAllocatorStatus ArraysCalloc(Allocator_t* allocator);

//================================================//

NodeAllocatorStatus AllocatorCtor(Allocator_t* allocator,
                                  size_t n_arrays,
                                  size_t n_nodes_in_array)
{
    VERIFY(!allocator,            return NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    allocator->n_arrays         = n_arrays;
    allocator->n_nodes_in_array = n_nodes_in_array;

    allocator->big_array        = (Node_t**) calloc(n_arrays, sizeof(Node_t**));
    VERIFY(!allocator->big_array, return NODE_ALLOCATORE_STD_CALLOC_ERROR);

    for (int i = 0; i < n_arrays; i++)
    {
        allocator->big_array[i] = (Node_t*) calloc (n_nodes_in_array, sizeof(Node_t*));
        VERIFY(!allocator->big_array[i], return NODE_ALLOCATORE_STD_CALLOC_ERROR);
    }

    allocator->answers = (Node_t*) calloc (n_nodes_in_array, sizeof(Node_t*));
    VERIFY(!allocator->answers, return NODE_ALLOCATORE_STD_CALLOC_ERROR);

    allocator->free_place = 0;

    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus AllocatorDtor(Allocator_t* allocator)
{
    VERIFY(!allocator, return NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    for (int i = 0; i < allocator->n_arrays; i++)
    {
        free(allocator->big_array[i]);
    }

    free(allocator->big_array);
    free(allocator->answers);

    allocator->free_place       = 0;
    allocator->n_arrays         = 0;
    allocator->n_nodes_in_array = 0;

    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus NodeCtor(Allocator* allocator, Node_t** new_node)
{
    VERIFY(!allocator, return NODE_ALLOCATOR_STRUCT_NULL_PTR_ERROR);
    VERIFY(!new_node,  return NODE_ALLOCATOR_INVALID_NEW_NODE_ERROR);

    if (allocator->free_place >= allocator->n_arrays * allocator->n_nodes_in_array)
    {
        VERIFY(BigArrayRealloc(allocator),
               return NODE_ALLOCATOR_BIG_ARRAY_REALLOC_ERROR);

        VERIFY(ArraysCalloc   (allocator),
               return NODE_ALLOCATOR_ARRAYS_CALLOC_ERROR);
    }

    size_t cur_array      = allocator->free_place / allocator->n_nodes_in_array;
    size_t rel_free_place = allocator->free_place % allocator->n_nodes_in_array;

    *new_node = &allocator->big_array[cur_array][rel_free_place];

    allocator->free_place++;

    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus BigArrayRealloc(Allocator_t* allocator)
{
    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//

NodeAllocatorStatus ArraysCalloc(Allocator_t* allocator)
{
    return NODE_ALLOCATOR_SUCCESS;
}

//================================================//
