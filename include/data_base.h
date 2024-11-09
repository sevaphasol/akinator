#ifndef DATA_BASE_H__
#define DATA_BASE_H__

//------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>

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

const char* const Delim = "\n\t";

//------------------------------------------------//
typedef enum DataBaseStatus
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
} DataBaseStatus;

//------------------------------------------------//

typedef struct DataBase
{
    FILE*       origin_file;
    FILE*       updated_file;
    size_t      size;
    char*       data;
    size_t      n_strings;
    char**      strings;
} DataBase_t;

//------------------------------------------------//

DataBaseStatus ReadDB   (DataBase_t* db, Node_t* root, const char* file_name);
DataBaseStatus UpdateDB (DataBase_t* db, Node_t* root, const char* file_name);

//------------------------------------------------//

#endif // DATA_BASE_H__
