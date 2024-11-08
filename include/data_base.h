#ifndef DATA_BASE_H__
#define DATA_BASE_H__

#include <stdio.h>

#include "akinator.h"

/* Provided for reducing code size of checks in functions */
#define VERIFY(condition, action)                      \
if (condition)                                         \
{                                                      \
    fprintf(stderr, "%s in %s:%d:%s\n",                \
    #action, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    action;                                            \
}                                                      \

const char* const DataBaseName = "DataBase.txt";

typedef enum DataBaseStatus
{
    DB_SUCCESS = 0,
    DB_FAILURE,
    DB_STRUCT_NULL_PTR_ERROR,
    DB_READ_FILE_ERROR,
    DB_NULL_PTR_ARG_ERROR,
    DB_GET_FILE_SIZE_ERROR,
    DB_FILE_OPEN_ERROR,
    DB_DATA_ALLOCATE_ERROR,
} DataBaseStatus;

typedef struct DataBase
{
    FILE*       file;
    size_t      size;
    char*       data;
    size_t      n_strings;
    char**      strings;
} DataBase_t;

DataBaseStatus ScanDB   (DataBase_t* db);
DataBaseStatus ReadDB   (DataBase_t* db, Node_t* root);
DataBaseStatus UpdateDB (DataBase_t* db, Node_t* root);

#endif // DATA_BASE_H__
