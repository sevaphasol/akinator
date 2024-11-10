#ifndef STRING_ALLOCATOR_H__
#define STRING_ALLOCATOR_H__

//————————————————————————————————————————————————//

const size_t MaxStrLen = 128;

//————————————————————————————————————————————————//

enum StringAllocatorStatus
{
    STRING_ALLOCATOR_SUCCESS = 0,
    STRING_ALLOCATOR_STRUCT_NULL_PTR_ERROR,
    STRING_ALLOCATORE_STD_CALLOC_ERROR,
    STRING_ALLOCATOR_INVALID_NEW_STRING_ERROR,
    STRING_ALLOCATOR_BIG_ARRAY_REALLOC_ERROR,
    STRING_ALLOCATOR_ARRAYS_CALLOC_ERROR,
};

//————————————————————————————————————————————————//

typedef struct String
{
    char str[MaxStrLen];
} String_t;

typedef struct StringAllocator
{
    size_t     n_arrays;
    size_t     n_strings_in_array;
    String_t** big_array;
    int        free_place;
} StringAllocator_t;

//————————————————————————————————————————————————//

StringAllocatorStatus StringAllocatorCtor (StringAllocator_t* string_allocator,
                                           size_t n_arrays,
                                           size_t n_strings_in_array);

StringAllocatorStatus StringAllocatorDtor (StringAllocator_t* string_allocator);

//------------------------------------------------//

StringAllocatorStatus GetStrPtr           (StringAllocator_t* string_allocator, char** ptr);

//————————————————————————————————————————————————//

#endif // STRING_ALLOCATOR_H__
