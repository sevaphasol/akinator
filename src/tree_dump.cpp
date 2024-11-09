#include <stdio.h>
#include <string.h>
#include <syscall.h>

#include "tree_dump.h"
#include "akinator.h"

TreeDumpStatus Dump(Node_t* root, int file_number)
{
    VERIFY(!root, return TREE_DUMP_STRUCT_NULL_PTR_ERROR);

    char dot_file_name[FileNameBufSize] = {};
    snprintf(dot_file_name, FileNameBufSize, "logs/dot_files/%03d.dot", file_number);

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
    snprintf(png_file_name, FileNameBufSize, "logs/images/%03d.png", file_number);

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

    fprintf(file, "elem%d["
                  "shape=\"Mrecord\", "
                  "label=\"{ %d | data.is_question = %s | "
                  "data.str = %s | left = %p | right = %p }"
                  "\"];\n",
                  *node_number, *node_number ,
                   node->data.is_question ? "true" : "false",
                   node->data.str, node->left, node->right);

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