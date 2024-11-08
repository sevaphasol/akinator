#ifndef DATA_BASE_H__
#define DATA_BASE_H__

#include <stdio.h>

#include "tree.h"

/* Provided for reducing code size of checks in functions */
#define DO_IF(condition, action)                       \
if (condition)                                         \
{                                                      \
    fprintf(stderr, "%s in %s:%d:%s\n",                \
    #action, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    action;                                            \
}                                                      \

const char* const DataBaseName = "DataBase.txt";

typedef enum DataBaseStatus
{
    DB_SUCCESS,
    DB_FAILURE,
    DB_NULL_PTR_ARG_ERROR,
    DB_GET_FILE_SIZE_ERROR,
} DataBaseStatus;

typedef struct DataBase
{
    FILE*       input;
    size_t      size;
    char*       data;
    size_t      n_strings;
    char**      strings;
} DataBase_t;

DataBaseStatus ScanDB   (DataBase_t* db);
DataBaseStatus ReadDB   (DataBase_t* db, Node_t* node, int* cur_string);
DataBaseStatus UpdateDB (DataBase_t* db, Node_t* node, int* level);

#endif // DATA_BASE_H__
