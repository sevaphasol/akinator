#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <data_base.h>

#include "akinator.h"
#include "tree_dump.h"

static AkinatorStatus RunGuessing   (Node_t* node);
static AkinatorStatus AskQuestion   (Node_t* node);
static AkinatorStatus JoinQuestion  (Node_t* node);
static int            PrintQuestion (const char* str, ...);

//================================================//

AkinatorStatus RunAkinator()
{
    Node_t*  root = NodeCtor();

    DataBase_t db = {};

    VERIFY(ReadDB(&db, root),   return AKINATOR_READ_DB_ERROR);

    RunGuessing(root); //TODO different modes

    VERIFY(UpdateDB(&db, root), return AKINATOR_UPDATE_DB_ERROR);

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus RunGuessing(Node_t* node)
{
    AskQuestion(node);

    Dump(node, 1);

    int ans  = PrintQuestion("Game is over. Do you want to play again?\n");

    if (ans == 'y') RunGuessing(node);

    return AKINATOR_SUCCESS;
}

//================================================//

Node_t* NodeCtor()
{
    return (Node*) calloc(1, sizeof(Node));
}

//================================================//

int PrintQuestion(const char* str, ...)
{
	va_list list;
    va_start(list, str);

    vprintf(str, list);
    int ans = getchar();
    getchar();

    return ans;
}

//================================================//

AkinatorStatus AskQuestion(Node_t* node)
{
    ASSERT(node);

    VERIFY(!node->data.str, return AKINATOR_NULL_ARG_PTR_ERROR);

    int ans = PrintQuestion("%s?\n", node->data.str);

    if (node->data.is_question)
    {
        (ans == 'y') ? AskQuestion(node->left) : AskQuestion(node->right);
    }
    else
    {
        if (ans != 'y') JoinQuestion(node);
    }

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus JoinQuestion(Node_t* node)
{
    VERIFY(!node, return AKINATOR_NULL_ARG_PTR_ERROR);

    printf("What is it then?\n");
    char ans[NodeDataStrSize]  = {};
    scanf("%[^\n]", ans);
    getchar();

    printf("What is the difference between %s and %s?\n", ans, node->data.str);
    char diff[NodeDataStrSize] = {};
    scanf("%[^\n]", diff);
    getchar();

    Node_t* left_node      = NodeCtor();
    VERIFY(!left_node, return AKINATOR_NODE_CTOR_ERROR);

    left_node->data.str    = strdup(ans);
    node->left             = left_node;
    node->left->level      = node->level + 1;

    Node_t* right_node     = NodeCtor();
    VERIFY(!right_node, return AKINATOR_NODE_CTOR_ERROR);

    right_node->data.str   = node->data.str;
    node->right            = right_node;
    node->right->level     = node->level + 1;

    node->data.str         = strdup(diff);
    node->data.is_question = true;

    return AKINATOR_SUCCESS;
}

//================================================//
