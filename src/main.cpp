#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "data_base.h"

int main()
{
    Node_t* root = NodeCtor();

    DataBase_t db = {};

    int cur_string = 0;

    ScanDB(&db);
    ReadDB(&db, root, &cur_string);

    PrintGraphTree(root, 1);

    StartGame(root);

    int level = 0;

    db.input = fopen(DataBaseName, "w");

    UpdateDB(&db, root, &level);

    fclose(db.input);

    return EXIT_SUCCESS;
}
