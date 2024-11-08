#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "tree.h"

static TreeReturnCode MakeDotNode    (Node_t* node, FILE* file, int* node_number);

static TreeReturnCode AskQuestion    (Node_t* node);
static TreeReturnCode JoinQuestion   (Node_t* node);

static int PrintQuestion(const char* str, ...);

//------------------------------------------------//

Node_t* NodeCtor()
{
    return (Node*) calloc(1, sizeof(Node));
}

//------------------------------------------------//

int PrintQuestion(const char* str, ...)
{
	va_list list;
    va_start(list, str);

    vprintf(str, list);
    int ans = getchar();
    getchar();

    return ans;
}

//------------------------------------------------//

TreeReturnCode AskQuestion(Node_t* node)
{
    DO_IF(!node || !node->data.str, return TREE_NULL_ARG_PTR_ERROR);

    int ans = PrintQuestion("%s?\n", node->data.str);

    if (node->data.is_question)
    {
        (ans == 'y') ? AskQuestion(node->left) : AskQuestion(node->right);
    }
    else
    {
        if (ans != 'y') JoinQuestion(node);
    }

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode JoinQuestion(Node_t* node)
{
    DO_IF(!node, return TREE_NULL_ARG_PTR_ERROR);

    printf("What is it then?\n");
    char ans[NodeDataStrSize]  = {};
    scanf("%[^\n]", ans);
    getchar();

    printf("What is the difference between %s and %s?\n", ans, node->data.str);
    char diff[NodeDataStrSize] = {};
    scanf("%[^\n]", diff);
    getchar();

    Node_t* left_node      = NodeCtor(); // check for nullptr
    left_node->data.str    = strdup(ans);
    node->left             = left_node;

    Node_t* right_node     = NodeCtor(); // check for nullptr
    right_node->data.str   = node->data.str;
    node->right            = right_node;

    node->data.str         = strdup(diff);
    node->data.is_question = true;

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode StartGame(Node_t* node)
{
    AskQuestion(node);

    PrintGraphTree(node, 1);

    int ans  = PrintQuestion("Game is over. Do you want to play again?\n");

    if (ans == 'y') StartGame(node);

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode PrintGraphTree(Node_t* node, int file_number)
{
    DO_IF(!node, return TREE_NULL_ARG_PTR_ERROR);

    char dot_file_name[FileNameBufSize] = {};
    snprintf(dot_file_name, FileNameBufSize, "logs/dot_files/%03d.dot", file_number);

    FILE* dot_file = fopen(dot_file_name, "w");

    fputs("digraph G{\n"
          "rankdir=HR;\n"
          "node[color=\"red\",fontsize=14];\n"
          "edge[color=\"#00eeee80\",fontsize=12, penwidth=1];\n",
          dot_file);

    int node_number = 1;

    MakeDotNode(node, dot_file, &node_number);

    fputs("}\n", dot_file);

    fclose(dot_file);

    char png_file_name[FileNameBufSize] = {};
    snprintf(png_file_name, FileNameBufSize, "logs/images/%03d.png", file_number);

    char command[SysCommandBufSize] = {};
    snprintf(command, SysCommandBufSize, "touch %s; dot %s -Tpng -o %s",
             png_file_name, dot_file_name, png_file_name);

    system(command);

    return TREE_SUCCESS;
}

//------------------------------------------------//

TreeReturnCode MakeDotNode(Node_t* node, FILE* file, int* node_number)
{
    DO_IF(!node, return TREE_NULL_ARG_PTR_ERROR);

    fprintf(file, "elem%d["
                  "shape=\"Mrecord\", "
                  "label=\"{ %d | data.is_question = %s | data.str = %s | left = %p | right = %p }"
                  "\"];\n",
                  *node_number, *node_number , node->data.is_question ? "true" : "false",
                   node->data.str, node->left, node->right);

    int head_node_number = *node_number;

    if (node->left)
    {
        int left_node_number = ++*node_number;

        fprintf(file, "elem%d->elem%d[label = \"yes\"];", head_node_number, left_node_number);

        MakeDotNode(node->left, file, node_number);
    }

    if (node->right)
    {
        int right_node_number = ++*node_number;

        fprintf(file, "elem%d->elem%d[label = \"no\"];", head_node_number, right_node_number);

        MakeDotNode(node->right, file, node_number);
    }

    return TREE_SUCCESS;
}

//------------------------------------------------//
