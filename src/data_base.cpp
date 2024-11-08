#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "data_base.h"
#include "tree.h"

static DataBaseStatus GetFileSize(FILE* file_ptr, size_t* size);

//------------------------------------------------//

DataBaseStatus ScanDB(DataBase_t* db)
{
    DO_IF(!db, return DB_NULL_PTR_ARG_ERROR);

    db->input = fopen(DataBaseName, "rb");

    GetFileSize(db->input, &db->size) != DB_SUCCESS;

    db->data = (char*) calloc(db->size, sizeof(char));

    fread(db->data, sizeof(char), db->size, db->input);

    fclose(db->input);

    char* cursor = db->data;
    while (cursor = strchr(cursor, '\n'))
    {
        db->n_strings++;
        *cursor++ = '\0';
    }

    db->strings = (char**) calloc(db->n_strings, sizeof(char*));

    cursor = db->data;
    for (int i = 0; i < db->n_strings; i++)
    {
        db->strings[i] = cursor;
        cursor = strchr(cursor, '\0') + 1;
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
    DO_IF(!file_ptr || !size, return DB_NULL_PTR_ARG_ERROR);

    struct stat file_status = {};

    DO_IF((fstat(fileno(file_ptr), &file_status) < 0), return DB_GET_FILE_SIZE_ERROR);

    *size = file_status.st_size;

    return DB_SUCCESS;
}

//------------------------------------------------//

DataBaseStatus ReadDB(DataBase_t* db, Node_t* node, int* cur_string)
{
    DO_IF(!db || !node || !cur_string, return DB_NULL_PTR_ARG_ERROR);

    DO_IF(!((0 <= *cur_string) && (*cur_string < db->n_strings)), return DB_NULL_PTR_ARG_ERROR);

    char character = 0;


    sscanf(db->strings[(*cur_string)++], "%c: %ms", &character, &node->data.str);

    if (character == 'a')
    {
        node->data.is_question = false;

        return DB_SUCCESS;
    }

    node->data.is_question = true;

    node->left = NodeCtor();
    ReadDB(db, node->left, cur_string);

    node->right = NodeCtor();
    ReadDB(db, node->right, cur_string);

    return DB_SUCCESS;
}

//------------------------------------------------//

DataBaseStatus UpdateDB(DataBase_t* db, Node_t* node, int* level)
{
    for (int i = 0; i < *level; i++)
    {
        fputc('\t', db->input);
    }

    if (node->data.is_question)
    {
        fprintf(db->input, "q:");
        fprintf(db->input, "%s\n", node->data.str);
        (*level)++;
        UpdateDB(db, node->left, level);
        (*level)++;
        UpdateDB(db, node->right, level);
    }
    else
    {
        fprintf(db->input, "a:");
        fprintf(db->input, "%s\n", node->data.str);
        (*level)--;
        return DB_SUCCESS;
    }

    (*level)--;

    return DB_SUCCESS;
}

//------------------------------------------------//
