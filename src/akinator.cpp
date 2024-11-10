#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//------------------------------------------------//

#include "akinator.h"
#include "data_base.h"
#include "colors.h"
#include "tree_dump.h"
#include "node_allocator.h"
#include "custom_assert.h"

//————————————————————————————————————————————————//

int   GetShortAnsColored (const char* color_code, const char* str, ...);
AkinatorStatus GetLongAnsColored  (StringAllocator_t* string_allocator,
                                   char*              ret_ptr,
                                   const char*        color_code,
                                   const char*        str, ...);

//------------------------------------------------//

static AkinatorStatus RunGuessing            (NodeAllocator_t*   node_allocator,
                                              StringAllocator_t* string_allocator,
                                              Node_t* node);

static AkinatorStatus RecursivelyAskQuestion (NodeAllocator_t*   node_allocator,
                                              StringAllocator_t* string_allocator,
                                              Node_t* node);

static AkinatorStatus JoinNewQuestion        (NodeAllocator_t*   node_allocator,
                                              StringAllocator_t* string_allocator,
                                              Node_t* node);

//------------------------------------------------//

static AkinatorStatus RunCharacteristic      (StringAllocator_t* string_allocator, Node_t* root);

//------------------------------------------------//

static AkinatorStatus RunDifference          (StringAllocator_t* string_allocator, Node_t* root);

//————————————————————————————————————————————————//

AkinatorStatus RunAkinator(NodeAllocator_t* node_allocator, StringAllocator_t* string_allocator)
{
    VERIFY(!node_allocator,
           return AKINATOR_NULL_ARG_PTR_ERROR);

    VERIFY(!string_allocator,
           return AKINATOR_NULL_ARG_PTR_ERROR);
    //------------------------------------------------//

    Node_t* root = nullptr;
    NodeCtor(node_allocator, &root);

    //------------------------------------------------//

    DataBase_t db = {};

    VERIFY(ReadDB(&db, node_allocator, string_allocator, root, DataBase),
           return AKINATOR_READ_DB_ERROR);

    Dump(root, DumpOriginDataBase);

    //------------------------------------------------//

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
                VERIFY(RunGuessing(node_allocator, string_allocator, root),
                       return AKINATOR_GUESSING_ERROR);
                break;

            case 'c':
                VERIFY(RunCharacteristic(string_allocator, root),
                       return AKINATOR_GUESSING_ERROR);
                break;

            case 'd':
                VERIFY(RunDifference(string_allocator, root),
                       return AKINATOR_GUESSING_ERROR);
                break;

            case 'q':
                running_akinator = false;
                break;

            default:
                ColorPrintf(RedColor, "\nIncorrect input. Try again");
                break;
        }
    }

    //------------------------------------------------//

    ans = GetShortAnsColored(TurquoiseColor, "\nUpdate data base?\n");

    if (ans == 'y')
    {
        VERIFY(UpdateDB(&db, root, DataBase),
               return AKINATOR_UPDATE_DB_ERROR);

        Dump(root, DumpUpdatedDataBase);
    }

    //------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//================================================//

int GetShortAnsColored(const char* color_code, const char* str, ...)
{
    ASSERT(color_code);
    ASSERT(str);

    //------------------------------------------------//

	va_list list;
    va_start(list, str);

    //------------------------------------------------//

	SetColor(color_code);
    vprintf(str, list);
	ResetColor();

    //------------------------------------------------//

    va_end(list);

    //------------------------------------------------//

    int ans = getchar();
    getchar();

    //------------------------------------------------//

    return ans;
}

//================================================//

AkinatorStatus GetLongAnsColored(StringAllocator_t* string_allocator,
                                 char**             ret_ptr,
                                 const char*        color_code,
                                 const char*        str, ...)
{
    ASSERT(string_allocator);
    ASSERT(ret_ptr);
    ASSERT(color_code);
    ASSERT(str);

    //------------------------------------------------//

	va_list list;
    va_start(list, str);

	SetColor(color_code);
    vprintf(str, list);
	ResetColor();

    va_end(list);

    //------------------------------------------------//

    VERIFY(GetStrPtr(string_allocator, ret_ptr),
           return AKINATOR_GET_STR_PTR_ERROR);

    scanf("%[^\n]", *ret_ptr);
    getchar();

    //------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus RunGuessing(NodeAllocator_t*   node_allocator,
                           StringAllocator_t* string_allocator,
                           Node_t* node)
{
    ASSERT(node);

    //------------------------------------------------//

    VERIFY(RecursivelyAskQuestion(node_allocator, string_allocator, node),
           return AKINATOR_ASK_QUESTION_ERROR);

    //------------------------------------------------//

    int ans = GetShortAnsColored(YellowColor, "\nGame is over. Do you want to play again?\n");

    if (ans == 'y') RunGuessing(node_allocator, string_allocator, node);

    //------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus RecursivelyAskQuestion(NodeAllocator_t*   node_allocator,
                                      StringAllocator_t* string_allocator,
                                      Node_t* node)
{
    ASSERT(node);

    //------------------------------------------------//

    VERIFY(!node->data.str,
           return AKINATOR_NULL_ARG_PTR_ERROR);

    //------------------------------------------------//

    int ans = GetShortAnsColored(YellowColor, "\n%s?\n", node->data.str);

    if (node->data.is_question)
    {
        if (ans == 'y')
        {
            RecursivelyAskQuestion(node_allocator, string_allocator, node->left);
        }
        else
        {
            RecursivelyAskQuestion(node_allocator, string_allocator, node->right);
        }
    }
    else
    {
        if (ans != 'y') JoinNewQuestion(node_allocator, string_allocator, node);
    }

    //------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus JoinNewQuestion(NodeAllocator_t*   node_allocator,
                               StringAllocator_t* string_allocator,
                               Node_t* node)
{
    ASSERT(node_allocator);
    ASSERT(string_allocator);
    ASSERT(node)

    //------------------------------------------------//

    char* ans = nullptr;

    GetLongAnsColored(string_allocator,
                      &ans,
                      PurpleColor,
                      "\nWhat is it then?\n");


    char* diff = nullptr;

    GetLongAnsColored(string_allocator,
                      &diff,
                      PurpleColor,
                      "\nWhat is the difference between %s and %s?\n",
                      ans, node->data.str);

    //------------------------------------------------//

    Node_t* left_node      = nullptr;
    NodeCtor(node_allocator, &left_node);
    VERIFY(!left_node,
           return AKINATOR_NODE_CTOR_ERROR);

    left_node->data.str    = ans;
    node->left             = left_node;
    node->left->level      = node->level + 1;

    Node_t* right_node     = nullptr;
    NodeCtor(node_allocator, &right_node);
    VERIFY(!left_node,
           return AKINATOR_NODE_CTOR_ERROR);

    right_node->data.str   = node->data.str;
    node->right            = right_node;
    node->right->level     = node->level + 1;

    node->data.str         = diff;
    node->data.is_question = true;

    //------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus RunCharacteristic(StringAllocator_t* string_allocator, Node_t* root)
{
    char* hero = nullptr;
    GetLongAnsColored(string_allocator,
                      &hero,
                      PurpleColor,
                      "\nWhose characteristic do you want me to tell?\n");

    return AKINATOR_SUCCESS;
}

//================================================//

AkinatorStatus RunDifference(StringAllocator_t* string_allocator, Node_t* root)
{
    char* hero1 = nullptr;
    char* hero2 = nullptr;

    GetLongAnsColored(string_allocator,
                      &hero1,
                      PurpleColor,
                      "\nWhich heroes difference do you want me to tell?\n");

    GetLongAnsColored(string_allocator,
                      &hero2,
                      PurpleColor,
                      "");

    return AKINATOR_SUCCESS;
}

//————————————————————————————————————————————————//
