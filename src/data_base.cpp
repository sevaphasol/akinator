#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <assert.h>

//-------------------------------------------------------------------//

#include "data_base.h"
#include "akinator.h"
#include "node_allocator.h"
#include "custom_assert.h"

//———————————————————————————————————————————————————————————————————//

static DataBaseStatus ScanDB                  (DataBase_t* db, const char*  file_name);

static DataBaseStatus DataBaseStringsCtor     (DataBase_t* db);

static DataBaseStatus GetFileSize             (FILE* file_ptr, size_t* size);

static size_t         CountNumberOfStrings    (char* text);

//-------------------------------------------------------------------//

static DataBaseStatus RemoveQuotes            (char** str);

static DataBaseStatus RecursivelyReadString   (DataBase_t*        db,
                                               NodeAllocator_t*   node_allocator,
                                               StringAllocator_t* string_allocator,
                                               Node_t*            parent,
                                               int*               cur_string);

static DataBaseStatus RecursivelyUpdateString (DataBase_t* db, Node_t* node);

//———————————————————————————————————————————————————————————————————//

DataBaseStatus ScanDB(DataBase_t* db, const char* file_name)
{
    ASSERT(db);
    ASSERT(file_name);

    //-------------------------------------------------------------------//

    db->origin_file = fopen(file_name, "rb");
    VERIFY(!db->origin_file,    return DB_FILE_OPEN_ERROR);

    //-------------------------------------------------------------------//

    VERIFY(GetFileSize(db->origin_file, &db->size),
                        return DB_FILE_OPEN_ERROR);

    db->data = (char*) calloc(db->size, sizeof(char));
    VERIFY(!db->data,    return DB_ALLOCATE_ERROR);

    VERIFY(fread(db->data, sizeof(char), db->size, db->origin_file) != db->size,
                        return DB_FILE_READ_ERROR);

    //-------------------------------------------------------------------//

    fclose(db->origin_file);

    //-------------------------------------------------------------------//

    return DB_SUCCESS;
}

//===================================================================//

DataBaseStatus DataBaseStringsCtor(DataBase_t* db)
{
    ASSERT(db);

    //-------------------------------------------------------------------//

    db->n_strings = CountNumberOfStrings(db->data);

    db->strings = (char**) calloc(db->n_strings, sizeof(char*));
    VERIFY(!db->data, return DB_ALLOCATE_ERROR);

    //-------------------------------------------------------------------//

    db->strings[0] = strtok(db->data, Delim);

    for (int i = 1; i < db->n_strings; i++)
    {
        db->strings[i] = strtok(nullptr, Delim);
    }

    //-------------------------------------------------------------------//

    return DB_SUCCESS;
}

//===================================================================//

size_t CountNumberOfStrings(char* text)
{
    ASSERT(text);

    //-------------------------------------------------------------------//

    int n_strings = 0;

    char* cursor = text;
    while (cursor = strchr(cursor, '\n'))
    {
        n_strings++;
        cursor++;
    }

    //-------------------------------------------------------------------//

    return n_strings;
}

//===================================================================//

DataBaseStatus GetFileSize(FILE* file_ptr, size_t* size)
{
    ASSERT(file_ptr);
    ASSERT(size);

    //-------------------------------------------------------------------//

    struct stat file_status = {};

    VERIFY((fstat(fileno(file_ptr), &file_status) < 0),
                        return DB_GET_FILE_SIZE_ERROR);

    *size = file_status.st_size;

    //-------------------------------------------------------------------//

    return DB_SUCCESS;
}

//===================================================================//

DataBaseStatus ReadDB(DataBase_t*        db,
                      NodeAllocator_t*   node_allocator,
                      StringAllocator_t* string_allocator,
                      Node_t*            root,
                      const char*        file_name)
{
    VERIFY(!db,
           return DB_STRUCT_NULL_PTR_ERROR);
    VERIFY(!root,
           return DB_NULL_PTR_ARG_ERROR);
    VERIFY(!file_name,
           return DB_NULL_PTR_ARG_ERROR);

    //-------------------------------------------------------------------//

    VERIFY(ScanDB(db, file_name),
           return DB_SCAN_FILE_ERROR);

    VERIFY(DataBaseStringsCtor(db),
           return DB_STRINGS_CTOR_ERROR);

    //-------------------------------------------------------------------//

    int cur_string = 0;

    VERIFY(RecursivelyReadString(db, node_allocator, string_allocator, root, &cur_string),
           return DB_READ_STRING_ERROR);

    // fprintf(stderr, "%d %d\n", cur_string, db->n_strings);
    // VERIFY(cur_string != db->n_strings,
    //        return DB_READ_FILE_ERROR);

    //-------------------------------------------------------------------//

    return DB_SUCCESS;
}

//===================================================================//

DataBaseStatus RecursivelyReadString(DataBase_t*        db,
                                     NodeAllocator_t*   node_allocator,
                                     StringAllocator_t* string_allocator,
                                     Node_t*            parent,
                                     int*               cur_string)
{
    ASSERT(node_allocator);
    ASSERT(db);
    ASSERT(parent);
    ASSERT(cur_string);

    //-------------------------------------------------------------------//

    VERIFY((*cur_string < 0) || (*cur_string >= db->n_strings),
           return DB_RECURSION_ERROR);

    //-------------------------------------------------------------------//

    GetStrPtr(string_allocator, &parent->data.str);

    while (!parent->data.str || parent->data.str[0] == '\0')
    {
        sscanf(db->strings[(*cur_string)++], "{ %[^\n]", parent->data.str);
    }

    if (strchr(parent->data.str, '}'))
    {
        VERIFY(RemoveQuotes(&parent->data.str), return DB_FORMAT_ERROR);

        node_allocator->answers[node_allocator->n_answers++] = parent;
        parent->data.is_question = false;

        return DB_SUCCESS;
    }

    //-------------------------------------------------------------------//

    parent->data.is_question = true;

    VERIFY(RemoveQuotes(&parent->data.str), return DB_FORMAT_ERROR);

    parent->left = nullptr;
    NodeCtor(node_allocator, &parent->left, parent, true);
    VERIFY(!parent->left,
           return DB_ALLOCATE_ERROR);

    parent->left->level  = parent->level + 1;
    VERIFY(RecursivelyReadString(db, node_allocator, string_allocator, parent->left,  cur_string),
                                 return DB_READ_STRING_ERROR);

    parent->right = nullptr;
    NodeCtor(node_allocator, &parent->right, parent, false);
    VERIFY(!parent->right,
           return DB_ALLOCATE_ERROR);

    parent->right->level = parent->level + 1;
    VERIFY(RecursivelyReadString(db, node_allocator, string_allocator, parent->right, cur_string),
                                 return DB_READ_STRING_ERROR);

    //-------------------------------------------------------------------//

    return DB_SUCCESS;
}

//===================================================================//

DataBaseStatus RemoveQuotes(char** str)
{
    ASSERT(str);
    ASSERT(*str);

    char* open_quote = strchr(*str, '"');
    VERIFY(!open_quote, return DB_FORMAT_ERROR);

    *str = strchr(*str, '"') + 1;

    char* close_quote = strchr(*str, '"');
    VERIFY(!close_quote, return DB_FORMAT_ERROR);
    *(close_quote) = '\0';

    return DB_SUCCESS;
}

//===================================================================//

DataBaseStatus UpdateDB(DataBase_t* db, Node_t* root, const char* file_name)
{
    VERIFY(!db,        return DB_STRUCT_NULL_PTR_ERROR);
    VERIFY(!root,      return DB_NULL_PTR_ARG_ERROR);
    VERIFY(!file_name, return DB_NULL_PTR_ARG_ERROR);

    //-------------------------------------------------------------------//

    db->updated_file = fopen(file_name, "w");

    //-------------------------------------------------------------------//

    VERIFY(RecursivelyUpdateString(db, root),
                    return DB_UPDATE_STRING_ERROR);

    //-------------------------------------------------------------------//

    fclose(db->updated_file);

    //-------------------------------------------------------------------//

    return DB_SUCCESS;
}

//===================================================================//

DataBaseStatus RecursivelyUpdateString(DataBase_t* db, Node_t* node)
{
    ASSERT(db);
    ASSERT(db->updated_file);
    ASSERT(node);

    //-------------------------------------------------------------------//

    VERIFY(!node->data.str, return DB_NULL_PTR_ARG_ERROR)

    //-------------------------------------------------------------------//

    for (int i = 0; i < node->level; i++)
    {
        fputc('\t', db->updated_file);
    }

    fprintf(db->updated_file, "{ \"%s\"%s", node->data.str, node->data.is_question ? "\n" : " }\n");

    //-------------------------------------------------------------------//

    if (node->data.is_question)
    {
        VERIFY(RecursivelyUpdateString(db, node->left),
                            return DB_UPDATE_STRING_ERROR);
        VERIFY(RecursivelyUpdateString(db, node->right),
                            return DB_UPDATE_STRING_ERROR);

        for (int i = 0; i < node->level; i++)
        {
            fputc('\t', db->updated_file);
        }

        fputc('}',  db->updated_file);
        fputc('\n', db->updated_file);
    }

    //-------------------------------------------------------------------//

    return DB_SUCCESS;
}

//———————————————————————————————————————————————————————————————————//
