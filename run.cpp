#include <cstdlib>
#include <printf.h>
#include "bTree.h"

int main() {
    BTree bt = static_cast<BTree>(malloc(sizeof(BTNode)));
    Queue q = static_cast<Queue>(malloc(sizeof(Queue)));
    initialize(bt);
    for (int i = 20; i <= 35; i++) {
        insertBTree(bt, i);
    }
    deleteBTree(bt, 29);
    deleteBTree(bt, 25);
//    deleteBTree(bt, 24);
//    deleteBTree(bt, 27);
//    deleteBTree(bt, 34);
//    deleteBTree(bt, 35);
    deleteBTree(bt, 23);
    deleteBTree(bt, 21);
    deleteBTree(bt, 22);
    deleteBTree(bt, 26);
    deleteBTree(bt, 20);
    deleteBTree(bt, 31);
    printfBTree(q, bt);
}
