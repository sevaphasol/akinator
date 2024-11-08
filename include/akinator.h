#ifndef AKINATOR_H__
#define AKINATOR_H__

#include <stdio.h>

//------------------------------------------------//

/* Provided for reducing code size of checks in functions */
#define VERIFY(condition, action)                      \
if (condition)                                         \
{                                                      \
    fprintf(stderr, "%s in %s:%d:%s\n",                \
    #action, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    action;                                            \
}                                                      \

//------------------------------------------------//

const size_t MinNumOfNodes     = 256;

const size_t FileNameBufSize   = 64;
const size_t SysCommandBufSize = 278;
const size_t NodeDataStrSize   = 128;

//------------------------------------------------//

enum AkinatorStatus
{
    AKINATOR_SUCCESS = 0,
    AKINATOR_NULL_ARG_PTR_ERROR,
    AKINATOR_READ_DB_ERROR,
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

Node_t*        NodeCtor  ();
AkinatorStatus Dump      (Node_t*  node, int file_number);

//------------------------------------------------//

#endif // AKINATOR_H__
