#include <cstdlib>
#include <printf.h>
#include "bTree.h"


int main() {
    BTree bt = static_cast<BTree>(malloc(sizeof(BTNode)));
    initialize(bt);
    insertBTree(bt, 11);
    insertBTree(bt, 12);
    insertBTree(bt, 13);
    insertBTree(bt, 14);
    insertBTree(bt, 15);
    insertBTree(bt, 16);
    insertBTree(bt, 17);
    insertBTree(bt, 18);
    insertBTree(bt, 19);
    insertBTree(bt, 20);
    printfBTree(bt);
}
