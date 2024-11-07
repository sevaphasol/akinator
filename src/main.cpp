#include <stdlib.h>

#include "tree.h"

int main()
{
    //------------------------------------------------//

    Node* root  = (Node*) calloc(1, sizeof(Node));

    //------------------------------------------------//

    Node* l0    = (Node*) calloc(1, sizeof(Node));
    Node* l00   = (Node*) calloc(1, sizeof(Node));
    Node* r01   = (Node*) calloc(1, sizeof(Node));
    JoinNode(l0, l00, r01);

    //------------------------------------------------//

    Node* r1    = (Node*) calloc(1, sizeof(Node));

    //------------------------------------------------//

    Node* l10   = (Node*) calloc(1, sizeof(Node));
    Node* l100  = (Node*) calloc(1, sizeof(Node));

    //------------------------------------------------//

    Node* r101  = (Node*) calloc(1, sizeof(Node));
    Node* l1010 = (Node*) calloc(1, sizeof(Node));
    Node* r1011 = (Node*) calloc(1, sizeof(Node));
    JoinNode(r101, l1010, r1011);

    //------------------------------------------------//
    JoinNode(l10, l100, r101);

    //------------------------------------------------//

    Node* r11   = (Node*) calloc(1, sizeof(Node));

    JoinNode(r1, l10, r11);

    //------------------------------------------------//

    JoinNode(root, l0, r1);

    //------------------------------------------------//

    PrintTextTree(root);
    PrintGraphTree(root, 1);

    return EXIT_SUCCESS;
}
