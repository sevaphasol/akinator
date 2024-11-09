#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "akinator.h"
#include "data_base.h"
#include "colors.h"
#include "tree_dump.h"

static AkinatorStatus RunGuessing            (Node_t* node);
static AkinatorStatus RecursivelyAskQuestion (Node_t* node);
static AkinatorStatus JoinNewQuestion        (Node_t* node);

//================================================//

Node_t* NodeCtor()
{
    return (Node*) calloc(1, sizeof(Node));
}

//================================================//

AkinatorStatus RunAkinator()
{
    Node_t*  root = NodeCtor();

    DataBase_t db = {};

    VERIFY(ReadDB(&db, root, DataBase), return AKINATOR_READ_DB_ERROR);

    Dump(root, DumpOriginDataBase);

    int ans = 0;

    bool running_akinator = true;

    while(running_akinator)
    {
        ans = GetShortAnsColored(YellowColor,
                                 "\nAkinator version 1.4.88\n"
                                 "If you want to play guessing       mode, press [g]uess\n"
                                 "If you want to play characteristic mode, press [c]haracteristic\n"
                                 "If you want to play differnce      mode, press [d]ifference\n"
                                 "If you want to      quit,                press [q]uit\n");

        switch (ans)
        {
            case 'g':
                VERIFY(RunGuessing(root), return AKINATOR_GUESSING_ERROR);
                break;

            case 'c':
                ColorPrintf(BlueColor, "\nDeveloping...\n");
                break;

            case 'd':
                ColorPrintf(BlueColor, "\nDeveloping...\n");
                break;

            case 'q':
                running_akinator = false;
                break;

            default:
                ColorPrintf(RedColor, "\nIncorrect input. Try again");
                break;
        }
    }

    ans = GetShortAnsColored(TurquoiseColor, "\nUpdate data base?\n");

    if (ans == 'y')
    {
        VERIFY(UpdateDB(&db, root, DataBase), return AKINATOR_UPDATE_DB_ERROR);

        Dump(root, DumpUpdatedDataBase);
    }

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus RunGuessing(Node_t* node)
{
    ASSERT(node);

    VERIFY(RecursivelyAskQuestion(node), return AKINATOR_ASK_QUESTION_ERROR);

    int ans = GetShortAnsColored(YellowColor, "\nGame is over. Do you want to play again?\n");

    if (ans == 'y') RunGuessing(node);

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus RecursivelyAskQuestion(Node_t* node)
{
    ASSERT(node);

    VERIFY(!node->data.str, return AKINATOR_NULL_ARG_PTR_ERROR);

    int ans = GetShortAnsColored(YellowColor, "\n%s?\n", node->data.str);

    if (node->data.is_question)
    {
        (ans == 'y') ? RecursivelyAskQuestion(node->left) : RecursivelyAskQuestion(node->right);
    }
    else
    {
        if (ans != 'y') JoinNewQuestion(node);
    }

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus JoinNewQuestion(Node_t* node)
{
    VERIFY(!node, return AKINATOR_NULL_ARG_PTR_ERROR);

    char* ans = GetLongAnsColored(PurpleColor,
                                  "\nWhat is it then?\n");

    char* diff = GetLongAnsColored(PurpleColor,
                                   "\nWhat is the difference between %s and %s?\n",
                                   ans, node->data.str);

    Node_t* left_node      = NodeCtor();
    VERIFY(!left_node, return AKINATOR_NODE_CTOR_ERROR);

    left_node->data.str    = ans;
    node->left             = left_node;
    node->left->level      = node->level + 1;

    Node_t* right_node     = NodeCtor();
    VERIFY(!right_node, return AKINATOR_NODE_CTOR_ERROR);

    right_node->data.str   = node->data.str;
    node->right            = right_node;
    node->right->level     = node->level + 1;

    node->data.str         = diff;
    node->data.is_question = true;

    return AKINATOR_SUCCESS;
}

//================================================//
