#ifndef TREE_DUMP_H__
#define TREE_DUMP_H__

//------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>

#include "akinator.h"

//------------------------------------------------//

#define LOGS_DIR "logs"
#define DOTS_DIR "dot_files"
#define PNGS_DIR "images"

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

TreeDumpStatus Dump (Node_t* root, const char* file_name);

//------------------------------------------------//

#endif // TREE_DUMP_H__
