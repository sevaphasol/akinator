#include <stdlib.h>
#include <string.h>

#include "tree.h"

int main()
{
    Node_t* root = NodeCtor();
    root->data.str = strdup("huy");

    StartGame(root);

    return EXIT_SUCCESS;
}
