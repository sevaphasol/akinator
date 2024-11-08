#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "data_base.h"
#include "akinator.h"

static DataBaseStatus GetFileSize             (FILE* file_ptr, size_t* size);

static DataBaseStatus DataBaseStringsCtor     (DataBase_t* db);

static DataBaseStatus RecursivelyReadString   (DataBase_t* db, Node_t* node, int* cur_string);
static DataBaseStatus RecursivelyUpdateString (DataBase_t* db, Node_t* node);

//------------------------------------------------//

DataBaseStatus ScanDB(DataBase_t* db) //TODO check for errors without making code look like govno
{
    VERIFY(!db, return DB_STRUCT_NULL_PTR_ERROR);

    db->file = fopen(DataBaseName, "rb");

    GetFileSize(db->file, &db->size);

    db->data = (char*) calloc(db->size, sizeof(char));

    fread(db->data, sizeof(char), db->size, db->file);

    fclose(db->file);

    return DB_SUCCESS;
}

//------------------------------------------------//

DataBaseStatus DataBaseStringsCtor(DataBase_t* db)
{
    VERIFY(!db, return DB_STRUCT_NULL_PTR_ERROR);

    char* cursor = db->data;
    while (cursor = strchr(cursor, '\n'))
    {
        db->n_strings++;
        cursor++;
    }

    db->strings = (char**) calloc(db->n_strings, sizeof(char*));

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

//------------------------------------------------//

DataBaseStatus GetFileSize(FILE* file_ptr, size_t* size)
{
    VERIFY(!file_ptr || !size, return DB_NULL_PTR_ARG_ERROR);

    struct stat file_status = {};

    VERIFY((fstat(fileno(file_ptr), &file_status) < 0), return DB_GET_FILE_SIZE_ERROR);

    *size = file_status.st_size;

    return DB_SUCCESS;
}

//------------------------------------------------//

DataBaseStatus ReadDB(DataBase_t* db, Node_t* root)
{
    VERIFY(!db || !root, return DB_NULL_PTR_ARG_ERROR);

    ScanDB(db);

    DataBaseStringsCtor(db);

    int cur_string = 0;

    RecursivelyReadString(db, root, &cur_string);

    VERIFY(cur_string != db->n_strings, return DB_READ_FILE_ERROR);

    return DB_SUCCESS;
}

//------------------------------------------------//

DataBaseStatus RecursivelyReadString(DataBase_t* db, Node_t* node, int* cur_string)
{
    VERIFY(!db || !node || !cur_string,                            return DB_NULL_PTR_ARG_ERROR);
    VERIFY(!((0 <= *cur_string) && (*cur_string < db->n_strings)), return DB_NULL_PTR_ARG_ERROR);

    char character = 0;

    sscanf(db->strings[(*cur_string)++], "%c: %m[^\n]", &character, &node->data.str);

    if (character == 'a')
    {
        node->data.is_question = false;

        return DB_SUCCESS;
    }

    node->data.is_question = true;

    node->left         = NodeCtor();
    node->left->level  = node->level + 1;
    RecursivelyReadString(db, node->left, cur_string);

    node->right        = NodeCtor();
    node->right->level = node->level + 1;
    RecursivelyReadString(db, node->right, cur_string);

    return DB_SUCCESS;
}

//------------------------------------------------//

DataBaseStatus UpdateDB(DataBase_t* db, Node_t* root)
{
    db->file = fopen(DataBaseName, "w");

    RecursivelyUpdateString(db, root);

    fclose(db->file);

    return DB_SUCCESS;
}

//------------------------------------------------//

DataBaseStatus RecursivelyUpdateString(DataBase_t* db, Node_t* node)
{
    for (int i = 0; i < node->level; i++)
    {
        fputc('\t', db->file);
    }

    if (node->data.is_question)
    {
        fprintf(db->file, "q: ");
        fprintf(db->file, "%s\n", node->data.str);

        RecursivelyUpdateString(db, node->left);
        RecursivelyUpdateString(db, node->right);
    }
    else
    {
        fprintf(db->file, "a: ");
        fprintf(db->file, "%s\n", node->data.str);

        return DB_SUCCESS;
    }

    return DB_SUCCESS;
}

//------------------------------------------------//
