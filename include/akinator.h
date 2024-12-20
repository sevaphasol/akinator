#ifndef AKINATOR_H__
#define AKINATOR_H__

//————————————————————————————————————————————————//

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------//

#include "node_allocator.h"
#include "string_allocator.h"

//————————————————————————————————————————————————//

const char* const DumpOriginDataBase  = "DataBase";
const char* const DumpUpdatedDataBase = "UpdatedDataBase";

const char* const DataBase            = "DataBase.txt";

//------------------------------------------------//

const size_t NumOfNodesInAllocatedArray   = 1024;
const size_t NumOfAllocatedNodeArrays     = 1;

const size_t NumOfStringsInAllocatedArray = 1024;
const size_t NumOfAllocatedStringArrays   = 1;

//————————————————————————————————————————————————//

enum AkinatorStatus
{
    AKINATOR_SUCCESS = 0,
    AKINATOR_NULL_ARG_PTR_ERROR,
    AKINATOR_READ_DB_ERROR,
    AKINATOR_UPDATE_DB_ERROR,
    AKINATOR_NODE_CTOR_ERROR,
    AKINATOR_ASK_QUESTION_ERROR,
    AKINATOR_GUESSING_ERROR,
    AKINATOR_GET_STR_PTR_ERROR,
    AKINATOR_NO_ANSWER_FOUND,
    AKINATOR_PRINT_HERO_CHARACTERISTIC_ERROR,
    AKINATOR_PUSH_HERO_CHARACTERISTIC_ERROR,
    AKINATOR_DUMP_ERROR,
};

//————————————————————————————————————————————————//

AkinatorStatus RunAkinator(NodeAllocator_t*   node_allocator,
                           StringAllocator_t* string_allocator);

//————————————————————————————————————————————————//

#endif // AKINATOR_H__
