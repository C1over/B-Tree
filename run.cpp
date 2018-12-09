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
    printfBTree(q, bt);
}
