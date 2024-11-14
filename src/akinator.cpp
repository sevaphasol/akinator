#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stack>

//-------------------------------------------------------------------//

#include "akinator.h"
#include "data_base.h"
#include "colors.h"
#include "tree_dump.h"
#include "node_allocator.h"
#include "custom_assert.h"

//———————————————————————————————————————————————————————————————————//

int                        GetShortAnsColored (const char* color_code, const char* str, ...);

AkinatorStatus             GetLongAnsColored  (StringAllocator_t* string_allocator,
                                               char*              ret_ptr,
                                               const char*        color_code,
                                               const char*        str, ...);

//-------------------------------------------------------------------//

static AkinatorStatus RunGuessing             (NodeAllocator_t*   node_allocator,
                                               StringAllocator_t* string_allocator,
                                               Node_t*            node);

static AkinatorStatus RecursivelyAskQuestion  (NodeAllocator_t*   node_allocator,
                                               StringAllocator_t* string_allocator,
                                               Node_t*            node);

static AkinatorStatus JoinNewQuestion         (NodeAllocator_t*   node_allocator,
                                               StringAllocator_t* string_allocator,
                                               Node_t*            node);

//-------------------------------------------------------------------//

static AkinatorStatus PushHeroCharacteristic  (NodeAllocator_t*      node_allocator,
                                               std::stack<Answer_t>* stk,
                                               Node_t*               hero_node);

//-------------------------------------------------------------------//

static AkinatorStatus RunCharacteristic       (NodeAllocator_t*      node_allocator,
                                               StringAllocator_t*    string_allocator,
                                               std::stack<Answer_t>* stk,
                                               Node_t*               root);

static AkinatorStatus FindAnswer              (NodeAllocator_t* node_allocator,
                                               char*            hero,
                                               Node_t**         ret_root);

static AkinatorStatus PrintHeroCharacteristic (std::stack<Answer_t>* stk, size_t level);


//------------------------------------------- -----//

static AkinatorStatus RunDifference           (NodeAllocator_t*      node_allocator,
                                               StringAllocator_t*    string_allocator,
                                               std::stack<Answer_t>* stk1,
                                               std::stack<Answer_t>* stk2,
                                               Node_t*               root);

static AkinatorStatus PrintHeroesDiffernce    (NodeAllocator_t*     node_allocator,
                                              std::stack<Answer_t>* stk1,
                                              std::stack<Answer_t>* stk2,
                                              Node_t*               hero1,
                                              Node_t*               hero2);

//———————————————————————————————————————————————————————————————————//

AkinatorStatus RunAkinator(NodeAllocator_t* node_allocator, StringAllocator_t* string_allocator)                                                                                                                                                                                                                        //TODO a, q -> y. n  + {} + distance between strings -> comment [] + Speak + Graphics // Espeak Festival // String structure
{
    VERIFY(!node_allocator,   return AKINATOR_NULL_ARG_PTR_ERROR);
    VERIFY(!string_allocator, return AKINATOR_NULL_ARG_PTR_ERROR);

    //-------------------------------------------------------------------//

    Node_t* root = nullptr;
    NodeCtor(node_allocator, &root, nullptr, false);

    //-------------------------------------------------------------------//

    DataBase_t db = {};

    VERIFY(ReadDB(&db, node_allocator, string_allocator, root, DataBase),
           return AKINATOR_READ_DB_ERROR);

    VERIFY(Dump(root, DumpOriginDataBase),
           return AKINATOR_DUMP_ERROR);

    //-------------------------------------------------------------------//

    std::stack<Answer_t> stk1 = {};
    std::stack<Answer_t> stk2 = {};

    //-------------------------------------------------------------------//

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
// ncurses vs SFML
        switch (ans)
        {
            case 'g':
                VERIFY(RunGuessing(node_allocator,
                                   string_allocator,
                                   root),
                       return AKINATOR_GUESSING_ERROR);
                break;

            case 'c':
                VERIFY(RunCharacteristic(node_allocator,
                                         string_allocator,
                                         &stk1,
                                         root),
                       return AKINATOR_GUESSING_ERROR);
                break;

            case 'd':
                VERIFY(RunDifference(node_allocator,
                                     string_allocator,
                                     &stk1, &stk2,
                                     root),
                       return AKINATOR_GUESSING_ERROR);
                break;

            case 'q':
                running_akinator = false;
                break;

            default:
                ColorPrint(RedColor, "\nIncorrect input. Try again");
                break;
        }
    }

    //-------------------------------------------------------------------//

    ans = GetShortAnsColored(TurquoiseColor, "\nUpdate data base?\n");

    if (ans == 'y')
    {
        VERIFY(UpdateDB(&db, root, DataBase),
               return AKINATOR_UPDATE_DB_ERROR);

    }
    Dump(root, DumpUpdatedDataBase);

    //-------------------------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//===================================================================//

int GetShortAnsColored(const char* color_code, const char* str, ...)
{
    ASSERT(color_code);
    ASSERT(str);

    //-------------------------------------------------------------------//

	va_list list;
    va_start(list, str);

	SetColor(color_code);
    vprintf(str, list);
	ResetColor();

    va_end(list);

    //-------------------------------------------------------------------//

    int ans = getchar();
    getchar();

    //-------------------------------------------------------------------//

    return ans;
}

//===================================================================//

AkinatorStatus GetLongAnsColored(StringAllocator_t* string_allocator,
                                 char**             ret_ptr,
                                 const char*        color_code,
                                 const char*        str, ...)
{
    ASSERT(string_allocator);
    ASSERT(ret_ptr);
    ASSERT(color_code);
    ASSERT(str);

    //-------------------------------------------------------------------//

	va_list list;
    va_start(list, str);

	SetColor(color_code);
    vprintf(str, list);
	ResetColor();

    va_end(list);

    //-------------------------------------------------------------------//

    VERIFY(GetStrPtr(string_allocator, ret_ptr),
           return AKINATOR_GET_STR_PTR_ERROR);

    scanf("%[^\n]", *ret_ptr);
    getchar();

    //-------------------------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus RunGuessing(NodeAllocator_t*   node_allocator,
                           StringAllocator_t* string_allocator,
                           Node_t*            node)
{
    ASSERT(node);

    //-------------------------------------------------------------------//

    VERIFY(RecursivelyAskQuestion(node_allocator,
                                  string_allocator,
                                  node),
           return AKINATOR_ASK_QUESTION_ERROR);

    //-------------------------------------------------------------------//

    int ans = GetShortAnsColored(YellowColor, "\nGame is over. Do you want to play again?\n");

    if (ans == 'y') RunGuessing(node_allocator, string_allocator, node);

    //-------------------------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus RecursivelyAskQuestion(NodeAllocator_t*   node_allocator,
                                      StringAllocator_t* string_allocator,
                                      Node_t*            node)
{
    ASSERT(node);

    //-------------------------------------------------------------------//

    VERIFY(!node->data.str,
           return AKINATOR_NULL_ARG_PTR_ERROR);

    //-------------------------------------------------------------------//

    int ans = GetShortAnsColored(PurpleColor, "\n%s?\n", node->data.str);

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

    //-------------------------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus JoinNewQuestion(NodeAllocator_t*   node_allocator,
                               StringAllocator_t* string_allocator,
                               Node_t*            node)
{
    ASSERT(node_allocator);
    ASSERT(string_allocator);
    ASSERT(node)

    //-------------------------------------------------------------------//

    char* ans = nullptr;

    GetLongAnsColored(string_allocator,
                      &ans,
                      PurpleColor,
                      "\nWhat is it then?\n"); //TODO check if such hero already exists

    char* diff = nullptr;

    GetLongAnsColored(string_allocator,
                      &diff,
                      PurpleColor,
                      "\nWhat is the difference between %s and %s?\n",
                      ans, node->data.str);

    //-------------------------------------------------------------------//

    Node_t* left_node      = nullptr;
    NodeCtor(node_allocator, &left_node, node, true);
    VERIFY(!left_node,
           return AKINATOR_NODE_CTOR_ERROR);

    left_node->data.str    = ans;
    node_allocator->answers[node_allocator->n_answers++] = left_node;
    node->left             = left_node;
    node->left->level      = node->level + 1;

    Node_t* right_node     = nullptr;
    NodeCtor(node_allocator, &right_node, node, false);
    VERIFY(!left_node,
           return AKINATOR_NODE_CTOR_ERROR);

    right_node->data.str   = node->data.str;
    node_allocator->answers[node_allocator->n_answers++] = right_node;
    node->right            = right_node;
    node->right->level     = node->level + 1;

    node->data.str         = diff;
    node->data.is_question = true;

    //-------------------------------------------------------------------//

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus PushHeroCharacteristic(NodeAllocator_t*      node_allocator,
                                      std::stack<Answer_t>* stk,
                                      Node_t*               hero_node)
{
    ASSERT(node_allocator);
    ASSERT(stk);
    ASSERT(hero_node);

    Answer_t prev_answer = {.node = hero_node};
    Answer_t cur_answer  = {.node = hero_node->parent};

    while (cur_answer.node)
    {
        cur_answer.is_positive = (cur_answer.node->left == prev_answer.node);

        (*stk).push(cur_answer);

        prev_answer = cur_answer;
        cur_answer  = {.node = cur_answer.node->parent};
    }

    cur_answer = {.node = hero_node};

    (*stk).push(cur_answer);

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus FindAnswer(NodeAllocator_t* node_allocator,
                          char*            hero,
                          Node_t**         ret_root)
{
    ASSERT(node_allocator);
    ASSERT(node_allocator->answers);
    ASSERT(hero);
    ASSERT(ret_root);

    for (int i = 0; i < node_allocator->n_answers; i++)
    {
        ASSERT(node_allocator->answers[i]);
        ASSERT(node_allocator->answers[i]->data.str);

        if (strcmp(hero, node_allocator->answers[i]->data.str) == 0)
        {
            *ret_root = node_allocator->answers[i];

            return AKINATOR_SUCCESS;
        }
    }

    return AKINATOR_NO_ANSWER_FOUND;
}

//===================================================================//

AkinatorStatus RunCharacteristic(NodeAllocator_t*      node_allocator,
                                 StringAllocator_t*    string_allocator,
                                 std::stack<Answer_t>* stk,
                                 Node_t*               root)
{
    ASSERT(node_allocator);
    ASSERT(string_allocator);
    ASSERT(stk);
    ASSERT(root);

    char* hero = nullptr;
    GetLongAnsColored(string_allocator,
                      &hero,
                      PurpleColor,
                      "\nWhose characteristic do you want me to tell?\n");

    Node_t* hero_node = nullptr;

    if (FindAnswer(node_allocator,
                   hero,
                   &hero_node) == AKINATOR_NO_ANSWER_FOUND)
    {
        ColorPrint(RedColor, "There is no %s in my database.\n", hero);

        return AKINATOR_SUCCESS;
    }

    VERIFY(PushHeroCharacteristic(node_allocator, stk, hero_node),
           return AKINATOR_PUSH_HERO_CHARACTERISTIC_ERROR);

    VERIFY(PrintHeroCharacteristic(stk, hero_node->level),
           return AKINATOR_PRINT_HERO_CHARACTERISTIC_ERROR);

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus PrintHeroCharacteristic(std::stack<Answer_t>* stk, size_t level)
{
    ASSERT(stk);

    bool positive_answer = true;

    ColorPrint(TurquoiseColor, "%s это ", ((*stk).top()).node->data.str);

    (*stk).pop();

    while (level != 0)
    {
        Answer_t cur_answer = (*stk).top();

        (*stk).pop();

        ColorPrint(TurquoiseColor, "%s%s ", cur_answer.is_positive ? "" : "не ", cur_answer.node->data.str);

        level--;
    }

    putchar('\n');

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus RunDifference(NodeAllocator_t*      node_allocator,
                             StringAllocator_t*    string_allocator,
                             std::stack<Answer_t>* stk1,
                             std::stack<Answer_t>* stk2,
                             Node_t*               root)
{
    ASSERT(node_allocator);
    ASSERT(string_allocator);
    ASSERT(root);

    //-------------------------------------------------------------------//

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

    //-------------------------------------------------------------------//

    Node_t* hero_node1 = nullptr;
    Node_t* hero_node2 = nullptr;

    if (FindAnswer(node_allocator,
                   hero1,
                   &hero_node1) == AKINATOR_NO_ANSWER_FOUND)
    {
        ColorPrint(RedColor, "There is no %s in my database.\n", hero1);

        return AKINATOR_SUCCESS;
    }

    if (FindAnswer(node_allocator,
                   hero2,
                   &hero_node2) == AKINATOR_NO_ANSWER_FOUND)
    {
        ColorPrint(RedColor, "There is no %s in my database.\n", hero2);

        return AKINATOR_SUCCESS;
    }

    //-------------------------------------------------------------------//

    PushHeroCharacteristic(node_allocator, stk1, hero_node1);
    PushHeroCharacteristic(node_allocator, stk2, hero_node2);

    PrintHeroesDiffernce(node_allocator, stk1, stk2, hero_node1, hero_node2);

    return AKINATOR_SUCCESS;
}

//===================================================================//

AkinatorStatus PrintHeroesDiffernce(NodeAllocator_t*      node_allocator,
                                    std::stack<Answer_t>* stk1,
                                    std::stack<Answer_t>* stk2,
                                    Node_t*               hero1,
                                    Node_t*               hero2)
{
    ASSERT(node_allocator);
    ASSERT(stk1);
    ASSERT(stk2);
    ASSERT(hero1);
    ASSERT(hero2);

    PrintHeroCharacteristic(stk1, hero1->level);
    PrintHeroCharacteristic(stk2, hero2->level);

    return AKINATOR_SUCCESS;
}

//===================================================================//

//———————————————————————————————————————————————————————————————————//
