#ifndef AKINATOR_H__
#define AKINATOR_H__

//------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>

#include "node_allocator.h"

//------------------------------------------------//

const char* const DumpOriginDataBase    = "DataBase";
const char* const DumpUpdatedDataBase   = "UpdatedDataBase";

const char* const DataBase              = "DataBase.txt";

const size_t NumOfNodesInAllocatedArray = 1024;
const size_t NumOfAllocatedArrays       = 1;

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

AkinatorStatus RunAkinator(Allocator_t* allocator);

//------------------------------------------------//

#endif // AKINATOR_H__
