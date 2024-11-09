#ifndef AKINATOR_H__
#define AKINATOR_H__

#include <stdio.h>
#include <stdlib.h>

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

const char* const DumpOriginDataBase  = "DataBase";
const char* const DumpUpdatedDataBase = "UpdatedDataBase";

const char* const DataBase            = "DataBase.txt";

const size_t MinNumOfNodes       = 256;
const size_t NodeDataStrSize     = 128;

//------------------------------------------------//

enum AkinatorStatus
{
    AKINATOR_SUCCESS = 0,
    AKINATOR_NULL_ARG_PTR_ERROR,
    AKINATOR_READ_DB_ERROR,
    AKINATOR_UPDATE_DB_ERROR,
    AKINATOR_NODE_CTOR_ERROR,
    AKINATOR_ASK_QUESTION_ERROR,
    AKINATOR_GUESSING_ERROR,
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

//------------------------------------------------//

AkinatorStatus RunAkinator();

//------------------------------------------------//

Node_t* NodeCtor();

//------------------------------------------------//

#endif // AKINATOR_H__
