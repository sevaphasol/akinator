#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <assert.h>

#include "data_base.h"
#include "akinator.h"

//------------------------------------------------//

static DataBaseStatus ScanDB                  (DataBase_t* db);
static DataBaseStatus GetFileSize             (FILE* file_ptr, size_t* size);
static DataBaseStatus DataBaseStringsCtor     (DataBase_t* db);

//------------------------------------------------//

static DataBaseStatus RecursivelyReadString   (DataBase_t* db, Node_t* node, int* cur_string);
static DataBaseStatus RecursivelyUpdateString (DataBase_t* db, Node_t* node);

//================================================//

DataBaseStatus ScanDB(DataBase_t* db)
{
    ASSERT(db);

    db->file = fopen(DataBaseName, "rb");
    VERIFY(!db->file,    return DB_FILE_OPEN_ERROR);

    VERIFY(GetFileSize(db->file, &db->size),
                        return DB_FILE_OPEN_ERROR);

    db->data = (char*) calloc(db->size, sizeof(char));
    VERIFY(!db->data,    return DB_ALLOCATE_ERROR);

    VERIFY(fread(db->data, sizeof(char), db->size, db->file) != db->size,
                        return DB_FILE_READ_ERROR);

    fclose(db->file);

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus DataBaseStringsCtor(DataBase_t* db)
{
    ASSERT(db);

    char* cursor = db->data;
    while (cursor = strchr(cursor, '\n'))
    {
        db->n_strings++;
        cursor++;
    }

    db->strings = (char**) calloc(db->n_strings, sizeof(char*));
    VERIFY(!db->data, return DB_ALLOCATE_ERROR);

    cursor = db->data;
    for (int i = 0; i < db->n_strings; i++)
    {
        db->strings[i] = cursor;
        cursor = strchr(cursor, '\n') + 1;
        while (isspace(*cursor))
        {
            cursor++;
        }
    }

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus GetFileSize(FILE* file_ptr, size_t* size)
{
    ASSERT(file_ptr);
    ASSERT(size);

    struct stat file_status = {};

    VERIFY((fstat(fileno(file_ptr), &file_status) < 0),
                        return DB_GET_FILE_SIZE_ERROR);

    *size = file_status.st_size;

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus ReadDB(DataBase_t* db, Node_t* root)
{
    ASSERT(db);
    ASSERT(root);

    VERIFY(ScanDB(db),
                    return DB_SCAN_FILE_ERROR);

    VERIFY(DataBaseStringsCtor(db),
                    return DB_STRINGS_CTOR_ERROR);

    int cur_string = 0;

    VERIFY(RecursivelyReadString(db, root, &cur_string),
                    return DB_READ_STRING_ERROR);

    VERIFY(cur_string != db->n_strings,
                    return DB_READ_FILE_ERROR);

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus RecursivelyReadString(DataBase_t* db, Node_t* node, int* cur_string)
{
    ASSERT(db);
    ASSERT(node);
    ASSERT(cur_string);

    VERIFY((*cur_string < 0) || (*cur_string >= db->n_strings),
                                    return DB_RECURSION_ERROR);

    char character = 0;

    sscanf(db->strings[(*cur_string)++], "%c: %m[^\n]", &character, &node->data.str);

    if (character == 'a')
    {
        node->data.is_question = false;

        return DB_SUCCESS;
    }

    VERIFY(character != 'q',        return DB_INPUT_FILE_FORMAT_ERROR);

    node->data.is_question = true;

    node->left         = NodeCtor();
    VERIFY(!node->left,             return DB_ALLOCATE_ERROR);
    node->left->level  = node->level + 1;
    VERIFY(RecursivelyReadString(db, node->left,  cur_string),
                                    return DB_READ_STRING_ERROR);

    node->right        = NodeCtor();
    node->right->level = node->level + 1;
    VERIFY(!node->right,            return DB_ALLOCATE_ERROR);
    VERIFY(RecursivelyReadString(db, node->right, cur_string),
                                    return DB_READ_STRING_ERROR);

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus UpdateDB(DataBase_t* db, Node_t* root)
{
    VERIFY(!db,     return DB_STRUCT_NULL_PTR_ERROR);
    VERIFY(!root,   return DB_NULL_PTR_ARG_ERROR);

    db->file = fopen(DataBaseName, "w");

    VERIFY(RecursivelyUpdateString(db, root),
                    return DB_UPDATE_STRING_ERROR);

    fclose(db->file);

    return DB_SUCCESS;
}

//================================================//

DataBaseStatus RecursivelyUpdateString(DataBase_t* db, Node_t* node)
{
    ASSERT(db);
    ASSERT(node);

    VERIFY(!node->data.str, return DB_NULL_PTR_ARG_ERROR)

    for (int i = 0; i < node->level; i++)
    {
        fputc('\t', db->file);
    }

    if (node->data.is_question)
    {
        fprintf(db->file, "q: ");
        fprintf(db->file, "%s\n", node->data.str);

        VERIFY(RecursivelyUpdateString(db, node->left),
                            return DB_UPDATE_STRING_ERROR);
        VERIFY(RecursivelyUpdateString(db, node->right),
                            return DB_UPDATE_STRING_ERROR);
    }
    else
    {
        fprintf(db->file, "a: ");
        fprintf(db->file, "%s\n", node->data.str);

        return DB_SUCCESS;
    }

    return DB_SUCCESS;
}

//================================================//
