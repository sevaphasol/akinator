#include <stdlib.h>

//------------------------------------------------//

#include "string_allocator.h"
#include "custom_assert.h"

//————————————————————————————————————————————————//

static StringAllocatorStatus BigArrayRealloc (StringAllocator_t* string_allocator);
static StringAllocatorStatus ArrayCalloc     (StringAllocator_t* string_allocator);

//————————————————————————————————————————————————//

StringAllocatorStatus StringAllocatorCtor(StringAllocator_t* string_allocator,
                                          size_t n_arrays,
                                          size_t n_strings_in_array)
{
    VERIFY(!string_allocator,
           return STRING_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    //------------------------------------------------//

    string_allocator->n_arrays  = n_arrays;

    string_allocator->big_array = (String_t**) calloc(n_arrays, sizeof(String_t**)); // buffer char**
    VERIFY(!string_allocator->big_array,
           return STRING_ALLOCATORE_STD_CALLOC_ERROR);

    //------------------------------------------------//

    string_allocator->n_strings_in_array = n_strings_in_array;

    for (int i = 0; i < n_arrays; i++)
    {
        string_allocator->big_array[i] = (String_t*) calloc (n_strings_in_array * MaxStrLen,
                                                  sizeof(String_t*));
        VERIFY(!string_allocator->big_array[i],
               return STRING_ALLOCATORE_STD_CALLOC_ERROR);
    }

    string_allocator->free_place = 0;

    //------------------------------------------------//

    return STRING_ALLOCATOR_SUCCESS;
}

//================================================//

StringAllocatorStatus StringAllocatorDtor(StringAllocator_t* string_allocator)
{
    VERIFY(!string_allocator,
           return STRING_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    //------------------------------------------------//

    for (int i = 0; i < string_allocator->n_arrays; i++)
    {
        free(string_allocator->big_array[i]);
    }

    free(string_allocator->big_array);

    string_allocator->free_place         = 0;
    string_allocator->n_arrays           = 0;
    string_allocator->n_strings_in_array = 0;

    //------------------------------------------------//

    return STRING_ALLOCATOR_SUCCESS;
}

//================================================//

StringAllocatorStatus GetStrPtr(StringAllocator* string_allocator, char** ptr)
{
    VERIFY(!string_allocator,
           return STRING_ALLOCATOR_STRUCT_NULL_PTR_ERROR);

    VERIFY(!ptr,
           return STRING_ALLOCATOR_INVALID_NEW_STRING_ERROR);


    //------------------------------------------------//

    if (string_allocator->free_place >= string_allocator->n_arrays * string_allocator->n_strings_in_array)
    {
        VERIFY(BigArrayRealloc(string_allocator),
               return STRING_ALLOCATOR_BIG_ARRAY_REALLOC_ERROR);

        VERIFY(ArrayCalloc(string_allocator),
               return STRING_ALLOCATOR_ARRAYS_CALLOC_ERROR);
    }

    //------------------------------------------------//

    size_t cur_array      = string_allocator->free_place / string_allocator->n_strings_in_array;
    size_t rel_free_place = string_allocator->free_place % string_allocator->n_strings_in_array;

    *ptr =  string_allocator->big_array[cur_array][rel_free_place].str;

    string_allocator->free_place++;

    //------------------------------------------------//

    return STRING_ALLOCATOR_SUCCESS;
}

//================================================//

StringAllocatorStatus BigArrayRealloc(StringAllocator_t* string_allocator)
{
    return STRING_ALLOCATOR_SUCCESS;
}

//================================================//

StringAllocatorStatus ArrayCalloc(StringAllocator_t* string_allocator)
{
    return STRING_ALLOCATOR_SUCCESS;
}

//————————————————————————————————————————————————//
