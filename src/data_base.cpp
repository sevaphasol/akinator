#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <assert.h>

//------------------------------------------------//

#include "data_base.h"
#include "akinator.h"
#include "node_allocator.h"
#include "custom_assert.h"

//————————————————————————————————————————————————//

static DataBaseStatus ScanDB                  (DataBase_t* db, const char* file_name);
static DataBaseStatus GetFileSize             (FILE* file_ptr, size_t* size);
static DataBaseStatus DataBaseStringsCtor     (DataBase_t* db);
static size_t         CountNumberOfStrings    (char* text);

//------------------------------------------------//

static DataBaseStatus RecursivelyReadString   (DataBase_t* db, Allocator_t* allocator,
                                               Node_t* node, int* cur_string);
static DataBaseStatus RecursivelyUpdateString (DataBase_t* db, Node_t* node);

//————————————————————————————————————————————————//

DataBaseStatus ScanDB(DataBase_t* db, const char* file_name)
{
    ASSERT(db);
    ASSERT(file_name);

    //------------------------------------------------//

    db->origin_file = fopen(file_name, "rb");
    VERIFY(!db->origin_file,    return DB_FILE_OPEN_ERROR);

    //------------------------------------------------//

    VERIFY(GetFileSize(db->origin_file, &db->size),
                        return DB_FILE_OPEN_ERROR);

    db->data = (char*) calloc(db->size, sizeof(char));
    VERIFY(!db->data,    return DB_ALLOCATE_ERROR);

    VERIFY(fread(db->data, sizeof(char), db->size, db->origin_file) != db->size,
                        return DB_FILE_READ_ERROR);

    //------------------------------------------------//

    fclose(db->origin_file);

    //------------------------------------------------//

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus DataBaseStringsCtor(DataBase_t* db)
{
    ASSERT(db);

    //------------------------------------------------//

    db->n_strings = CountNumberOfStrings(db->data);

    db->strings = (char**) calloc(db->n_strings, sizeof(char*));
    VERIFY(!db->data, return DB_ALLOCATE_ERROR);

    //------------------------------------------------//

    db->strings[0] = strtok(db->data, Delim);

    for (int i = 1; i < db->n_strings; i++)
    {
        db->strings[i] = strtok(nullptr, Delim);
    }

    //------------------------------------------------//

    return DB_SUCCESS;
}

//================================================//

size_t CountNumberOfStrings(char* text)
{
    ASSERT(text);

    //------------------------------------------------//

    int n_strings = 0;

    char* cursor = text;
    while (cursor = strchr(cursor, '\n'))
    {
        n_strings++;
        cursor++;
    }

    //------------------------------------------------//

    return n_strings;
}

//================================================//

DataBaseStatus GetFileSize(FILE* file_ptr, size_t* size)
{
    ASSERT(file_ptr);
    ASSERT(size);

    //------------------------------------------------//

    struct stat file_status = {};

    VERIFY((fstat(fileno(file_ptr), &file_status) < 0),
                        return DB_GET_FILE_SIZE_ERROR);

    *size = file_status.st_size;

    //------------------------------------------------//

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus ReadDB(DataBase_t* db, Allocator_t* allocator,
                      Node_t* root, const char* file_name)
{
    VERIFY(!db,
           return DB_STRUCT_NULL_PTR_ERROR);
    VERIFY(!root,
           return DB_NULL_PTR_ARG_ERROR);
    VERIFY(!file_name,
           return DB_NULL_PTR_ARG_ERROR);

    //------------------------------------------------//

    VERIFY(ScanDB(db, file_name),
           return DB_SCAN_FILE_ERROR);

    VERIFY(DataBaseStringsCtor(db),
           return DB_STRINGS_CTOR_ERROR);

    //------------------------------------------------//

    int cur_string = 0;

    VERIFY(RecursivelyReadString(db, allocator, root, &cur_string),
           return DB_READ_STRING_ERROR);

    VERIFY(cur_string != db->n_strings,
           return DB_READ_FILE_ERROR);

    //------------------------------------------------//

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus RecursivelyReadString   (DataBase_t* db, Allocator_t* allocator,
                                        Node_t* node, int* cur_string)
{
    ASSERT(allocator);
    ASSERT(db);
    ASSERT(node);
    ASSERT(cur_string);

    //------------------------------------------------//

    VERIFY((*cur_string < 0) || (*cur_string >= db->n_strings),
                                    return DB_RECURSION_ERROR);

    //------------------------------------------------//

    char character = 0;

    sscanf(db->strings[(*cur_string)++], "%c: %m[^\n]", &character, &node->data.str);

    if (character == 'a')
    {
        allocator->answers[allocator->n_answers++] = node;
        node->data.is_question = false;

        return DB_SUCCESS;
    }

    VERIFY(character != 'q',
           return DB_INPUT_FILE_FORMAT_ERROR);

    //------------------------------------------------//

    node->data.is_question = true;

    node->left = nullptr;
    NodeCtor(allocator, &node->left);
    VERIFY(!node->left,             return DB_ALLOCATE_ERROR);

    node->left->level  = node->level + 1;
    VERIFY(RecursivelyReadString(db, allocator, node->left,  cur_string),
                                    return DB_READ_STRING_ERROR);

    node->right = nullptr;
    NodeCtor(allocator, &node->right);
    VERIFY(!node->right,            return DB_ALLOCATE_ERROR);

    node->right->level = node->level + 1;
    VERIFY(RecursivelyReadString(db, allocator, node->right, cur_string),
                                    return DB_READ_STRING_ERROR);

    //------------------------------------------------//

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus UpdateDB(DataBase_t* db, Node_t* root, const char* file_name)
{
    VERIFY(!db,        return DB_STRUCT_NULL_PTR_ERROR);
    VERIFY(!root,      return DB_NULL_PTR_ARG_ERROR);
    VERIFY(!file_name, return DB_NULL_PTR_ARG_ERROR);

    //------------------------------------------------//

    db->updated_file = fopen(file_name, "w");

    //------------------------------------------------//

    VERIFY(RecursivelyUpdateString(db, root),
                    return DB_UPDATE_STRING_ERROR);

    //------------------------------------------------//

    fclose(db->updated_file);

    //------------------------------------------------//

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus RecursivelyUpdateString(DataBase_t* db, Node_t* node)
{
    ASSERT(db);
    ASSERT(node);

    //------------------------------------------------//

    VERIFY(!node->data.str, return DB_NULL_PTR_ARG_ERROR)

    //------------------------------------------------//

    for (int i = 0; i < node->level; i++)
    {
        fputc('\t', db->updated_file);
    }

    if (node->data.is_question)
    {
        fprintf(db->updated_file, "q: ");
        fprintf(db->updated_file, "%s\n", node->data.str);

        VERIFY(RecursivelyUpdateString(db, node->left),
                            return DB_UPDATE_STRING_ERROR);
        VERIFY(RecursivelyUpdateString(db, node->right),
                            return DB_UPDATE_STRING_ERROR);
    }
    else
    {
        fprintf(db->updated_file, "a: ");
        fprintf(db->updated_file, "%s\n", node->data.str);

        return DB_SUCCESS;
    }

    //------------------------------------------------//

    return DB_SUCCESS;
}

//————————————————————————————————————————————————//
