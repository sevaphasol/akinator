#ifndef DATA_BASE_H__
#define DATA_BASE_H__

//————————————————————————————————————————————————//

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------//

#include "akinator.h"
#include "node_allocator.h"
#include "string_allocator.h"

//————————————————————————————————————————————————//

const char* const Delim = "\n\t";

//————————————————————————————————————————————————//

enum DataBaseStatus
{
    DB_SUCCESS = 0,
    DB_FAILURE,
    DB_RECURSION_ERROR,
    DB_FILE_READ_ERROR,
    DB_SCAN_FILE_ERROR,
    DB_STRINGS_CTOR_ERROR,
    DB_INPUT_FILE_FORMAT_ERROR,
    DB_READ_STRING_ERROR,
    DB_UPDATE_STRING_ERROR,
    DB_STRUCT_NULL_PTR_ERROR,
    DB_READ_FILE_ERROR,
    DB_NULL_PTR_ARG_ERROR,
    DB_GET_FILE_SIZE_ERROR,
    DB_ALLOCATE_ERROR,
    DB_FILE_OPEN_ERROR,
    DB_DATA_ALLOCATE_ERROR,
};

//————————————————————————————————————————————————//

typedef struct DataBase
{
    FILE*       origin_file;
    FILE*       updated_file;

    size_t      size;
    char*       data;

    size_t      n_strings;
    char**      strings;
} DataBase_t;

//————————————————————————————————————————————————//

DataBaseStatus ReadDB   (DataBase_t*        db,
                         NodeAllocator_t*   node_allocator,
                         StringAllocator_t* string_allocator,
                         Node_t*            root,
                         const char*        file_name);

DataBaseStatus UpdateDB (DataBase_t* db,
                         Node_t*     root,
                         const char* file_name);

//————————————————————————————————————————————————//

#endif // DATA_BASE_H__
