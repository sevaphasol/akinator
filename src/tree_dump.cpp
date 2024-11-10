#include <stdio.h>
#include <string.h>
#include <syscall.h>

#include "tree_dump.h"
#include "akinator.h"
#include "custom_assert.h"

static TreeDumpStatus MakeDotNode (Node_t* node, FILE* file, int* node_number);

//================================================//

TreeDumpStatus Dump(Node_t* root, const char* file_name)
{
    VERIFY(!root, return TREE_DUMP_STRUCT_NULL_PTR_ERROR);

    char dot_file_name[FileNameBufSize] = {};
    snprintf(dot_file_name, FileNameBufSize, LOGS_DIR "/" DOTS_DIR "/" "%s.dot", file_name);

    FILE* dot_file = fopen(dot_file_name, "w");

    VERIFY(!dot_file, return TREE_DUMP_FILE_OPEN_ERROR);

    fprintf(dot_file, "digraph G{\n"
                      "rankdir=HR;\n"
                      "node[style=filled, color=\"%s\", fillcolor=\"%s\","
                      "fontcolor=\"%s\", fontsize=14];\n"
                      "edge[color=\"%s\", fontsize=12, penwidth=1, "
                      "fontcolor = \"%s\"];\n"
                      "bgcolor=\"%s\";\n",
                      NodeBorderColor, NodeBackGroundColor, NodeFontColor,
                      EdgeColor, EdgeFontColor,
                      BackGroundColor);

    int node_number = 1;

    MakeDotNode(root, dot_file, &node_number);

    fputs("}\n", dot_file);

    fclose(dot_file);

    char png_file_name[FileNameBufSize] = {};
    snprintf(png_file_name, FileNameBufSize, LOGS_DIR "/" PNGS_DIR "/" "%s.png", file_name);

    char command[SysCommandBufSize] = {};
    snprintf(command, SysCommandBufSize, "touch %s; dot %s -Tpng -o %s",
             png_file_name, dot_file_name, png_file_name);

    system(command);

    return TREE_DUMP_SUCCESS;
}

//================================================//

TreeDumpStatus MakeDotNode(Node_t* node, FILE* file, int* node_number)
{
    ASSERT(node);
    ASSERT(file);
    ASSERT(node_number);

    if (node->data.is_question)
    {
        fprintf(file, "elem%d["
                      "shape=\"Mrecord\", "
                      "label=\"%s?\""
                      "];\n",
                      *node_number,
                      node->data.str);
    }
    else
    {
        fprintf(file, "elem%d["
                      "shape=\"Mrecord\", "
                      "label=\"%s\""
                      "];\n",
                      *node_number,
                      node->data.str);
    }

    int head_node_number = *node_number;

    if (node->left)
    {
        int left_node_number = ++*node_number;

        fprintf(file, "elem%d->elem%d[label = \" да \"];",
                      head_node_number, left_node_number);

        MakeDotNode(node->left, file, node_number);
    }

    if (node->right)
    {
        int right_node_number = ++*node_number;

        fprintf(file, "elem%d->elem%d[label = \" нет \"];",
                       head_node_number, right_node_number);

        MakeDotNode(node->right, file, node_number);
    }

    return TREE_DUMP_SUCCESS;
}

//================================================//
