#ifndef TREE_DUMP_H__
#define TREE_DUMP_H__

#include <stdio.h>
#include <stdlib.h>

#include "akinator.h"

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

const size_t FileNameBufSize          = 64;
const size_t SysCommandBufSize        = 278;
const char* const BackGroundColor     = "#bcbddc";
const char* const NodeBackGroundColor = "#a1d99b";
const char* const NodeBorderColor     = "#756bb1";
const char* const EdgeColor           = "#006d2c";
const char* const NodeFontColor       = "#ba3d3d";
const char* const EdgeFontColor       = "#49006a";

//------------------------------------------------//

enum TreeDumpStatus
{
    TREE_DUMP_SUCCESS = 0,
    TREE_DUMP_STRUCT_NULL_PTR_ERROR,
    TREE_DUMP_FILE_OPEN_ERROR,
};

//------------------------------------------------//

TreeDumpStatus Dump        (Node_t* root, int file_number);
TreeDumpStatus MakeDotNode (Node_t* node, FILE* file, int* node_number);

//------------------------------------------------//

#endif // TREE_DUMP_H__
