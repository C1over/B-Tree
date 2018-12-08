#include <cstdlib>
#include "bTree.h"

int search(BTree bt, KeyType key);


void searchBTree(BTree bt, KeyType key, Result result) {
    int order = 0;
    bool found = false;
    BTree p = bt, q = NULL;
    while (p != NULL && !found) {
        order = search(p, key);
        if (order <= p->keyNum && p->key[order] == key) {
            found = true;
        } else {
            q = p;
            p = p->child[order];
        }
    }
    if (found) {
        result->ptr = p;
        result->order = order;
        result->tag = true;
    } else {
        result->ptr = q;
        result->order = order;
        result->tag = false;
    }
}

/**
 *  在bt->key[0..p->keyNum-1]找关键字
 * @param bt  查找的B树节点
 * @param k   关键字
 * @return    关键字在数组中的位置
 */
int search(BTree bt, KeyType k) {
    int order = 0;
    while (order <= bt->keyNum && k > bt->key[order]) {
        order++;
    }
    return order;
}


/**
 *  生成一个新的根节点
 * @param t      新节点
 * @param p      关键字的左节点
 * @param key    关键字
 * @param ap     关键字的右节点
 */
void newRoot(BTree &t, BTree p, KeyType key, BTree ap) {
    t = static_cast<BTree>(malloc(sizeof(BTNode)));
    t->keyNum = 1;
    t->child[0] = p;
    t->child[1] = ap;
    t->key[0] = key;
    if (p != NULL) {
        p->parent = t;
    }
    if (ap != NULL) {
        ap->parent = t;
    }
    t->parent = NULL;
}

/**
 *  将关键字和新节点插入到q->key[pos]和q->child[pos]
 * @param q      插入节点
 * @param pos    插入位置
 * @param key    关键字
 * @param ap     新节点
 */
void insert(BTree &q, int pos, int key, BTree ap) {
    int n = q->keyNum;
    for (int j = n; j >= pos; j++) {
        q->key[j + 1] = q->key[j];
        q->child[j + 1] = q->child[j];
    }
    q->key[pos] = key;
    q->child[pos] = ap;
    if (ap != NULL) {
        ap->parent = q;
        q->keyNum++;
    }
}

/**
 *  将q节点分裂成两个节点，前一半保存在原节点，后一半移入ap所指新节点
 * @param q    原节点
 * @param ap   新节点
 */
void split(BTree &q, BTree &ap) {
    int mid = (STAGE + 1) / 2;
    int n = q->keyNum;
    ap = static_cast<BTree>(malloc(sizeof(BTNode)));
    for (int i = mid, j = 0; i < n; i++, j++) {
        ap->key[j] = q->key[i];
        ap->child[j] = q->child[i];
    }
    ap->child[mid] = q->child[n];
    ap->keyNum = n - mid;
    ap->parent = q->parent;
    for (int i = 0; i <= n - mid; i++) {
        if (ap->child[i] != NULL) {
            ap->child[i]->parent = ap;
        }
    }
    q->keyNum = mid - 1;
}

void insertBTree(BTree &bt, KeyType key) {
    bool finished = false;
    bool needNewRoot = false;
    Result r = static_cast<Result>(malloc(sizeof(Result)));
    searchBTree(bt, key, r);
    BTree q = r->ptr;
    int order = r->order;
    if (q == NULL) {
        newRoot(bt, NULL, key, NULL);
    } else {
        int x = key;
        BTree ap = NULL;
        while (!needNewRoot && !finished) {
            insert(q, order, x, ap);
            if (q->keyNum < STAGE) {
                finished = true;
            } else {
                split(q, ap);
                x = q->key[STAGE / 2];
                if (q->parent != NULL) {
                    q = q->parent;
                    order = search(bt, x);
                } else {
                    needNewRoot = true;
                }
            }
        }
        if (needNewRoot) {
            newRoot(bt, q, x, ap);
        }
    }
}


void deleteBTree(BTree &bt, KeyType key) {

}

void destroyBTree(BTree &bt) {
    if (bt == NULL) {
        return;
    }
    for (int i = 0; i <= bt->keyNum; i++) {
        if (!bt->child[i]) {
            destroyBTree(bt->child[i]);
        }
    }
    free(bt);
}

void initialize(BTree &bt) {
    bt = static_cast<BTree>(malloc(sizeof(BTNode)));
}