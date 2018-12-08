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
            p = p->child[order - 1];
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
 *  在bt->key[1..p->keyNum-1]找关键字
 * @param bt  查找的B树节点
 * @param k   关键字
 * @return    关键字在数组中的位置
 */
int search(BTree bt, KeyType k) {
    int order = 1;
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
    t->key[1] = key;
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
    ap->child[0] = q->child[mid];
    for (int i = mid + 1, j = 1; i <= n; i++, j++) {
        ap->key[j] = q->key[i];
        ap->child[j] = q->child[i];
    }
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
    if (!q) {
        newRoot(bt, NULL, key, NULL);
    } else {
        KeyType x = key;
        BTree ap = NULL;
        while (!needNewRoot && !finished) {
            insert(q, order, x, ap);
            if (q->keyNum < STAGE) {
                finished = true;
            } else {
                split(q, ap);
                x = q->key[(STAGE + 1) / 2];
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

/**
 *  作用：当被删除关键字Ki不是最下层非终端节点时
 *  实现：从Ai子树中找出位于最下层非终端节点的最小关键字替代Ki被删除
 * @param p        当前节点
 * @param order    关键字位序
 */
void successor(BTree &p, int order) {
    BTree leaf = p;
    if (!p) {
        return;
    }
    leaf = leaf->child[order];
    while (!leaf->child[0]) {
        leaf = leaf->child[0];
    }
    p->key[order] = leaf->key[0];
    p = leaf;
}

/**
 *  从传入节点移除关键字key[order]
 * @param p       待删除节点
 * @param order   关键字位序
 */
void remove(BTree &p, int order) {
    for (int k = order; k < p->keyNum - 1; k++) {
        p->key[k] = p->key[k + 1];
        p->child[k + 1] = p->child[k + 2];
    }
    p->keyNum--;
}

/**
 * 向兄弟节点借关键字
 * @param p               B树结点
 * @param leftBrother     左兄弟结点
 * @param rightBrother    右兄弟节点
 * @param parent          双亲节点
 * @param order           关键字位序
 */
void borrowFromBrother(BTree &p, BTree &leftBrother,
                       BTree &rightBrother, BTree &parent, int &order) {
    if (leftBrother != NULL && leftBrother->keyNum >= (STAGE + 1) / 2) {
        for (int j = p->keyNum + 1; j > 0; j--) {
            // 关键字与指针后移，腾出第一个位置
            if (j > 1) {
                p->key[j] = p->key[j - 1];
            }
            p->child[j] = p->child[j - 1];
        }
        p->child[0] = leftBrother->child[leftBrother->keyNum];
        if (p->child[0] != NULL) {
            p->child[0]->parent = p;
        }
        leftBrother->child[leftBrother->keyNum] = NULL;
        p->key[0] = parent->key[order];
        parent->key[order] = leftBrother->key[leftBrother->keyNum];
        leftBrother->keyNum--;
        p->keyNum++;
    } else if (rightBrother && rightBrother->keyNum >= (STAGE + 1) / 2) {
        p->key[p->keyNum] = parent->key[order + 1];
        // 子树指针指向右兄弟最左边的子树指针
        p->child[p->keyNum + 1] = rightBrother->child[0];
        if (p->child[p->keyNum + 1] != NULL) {
            p->child[p->keyNum + 1]->parent = p;
        }
        p->keyNum++;
        parent->key[order + 1] = rightBrother->key[0];
        for (int j = 0; j < rightBrother->keyNum; j++) {
            if (j > 0) {
                rightBrother->key[j] = rightBrother->key[j + 1];
            }
            rightBrother->child[j] = rightBrother->child[j + 1];
        }
        rightBrother->child[rightBrother->keyNum] = NULL;
        rightBrother->keyNum--;
    }
}

void mergeWithLeftBrother(BTree &leftBrother, BTree &parent, BTree &p, BTree &bt, int &order);

void mergeWithRightBrother(BTree &rightBrother, BTree &parent, BTree &p, BTree &bt, int &order);

/**
 * 调整B树
 * @param bt  B树
 * @param p   待调整节点
 */
void restore(BTree &bt, BTree &p) {
    BTree parent, leftBrother, rightBrother;
    parent = p->parent;
    if (parent) {
        int pos;
        for (pos = 0; pos <= parent->keyNum; pos++) {
            if (parent->child[pos] == p) {
                break;
            }
        }
        if (pos > 0) {
            leftBrother = parent->child[pos - 1];
        } else {
            leftBrother = NULL;
        }
        if (pos < parent->keyNum) {
            rightBrother = parent->child[pos + 1];
        } else {
            rightBrother = NULL;
        }
        if ((leftBrother != NULL && leftBrother->keyNum >= (STAGE + 1) / 2) ||
            (rightBrother != NULL && rightBrother->keyNum >= (STAGE + 1) / 2)) {
            borrowFromBrother(p, leftBrother, rightBrother, parent, pos);
        } else {
            if (leftBrother) {
                mergeWithLeftBrother(leftBrother, parent, p, bt, pos);
            } else if (rightBrother) {
                mergeWithRightBrother(rightBrother, parent, p, bt, pos);
            } else {
                for (int j = 0; j <= p->keyNum + 1; j++) {
                    if (p->child[j] != NULL) {
                        bt = p->child[j];
                        break;
                    }
                }
                bt->parent = NULL;
            }
        }
    } else {
        BTree a;
        for (int j = 0; j <= p->keyNum + 1; j++) {
            if (p->child[j] != NULL) {
                a = p;
                p = p->child[j];
                a->child[j] = NULL;
                free(a);
                break;
            }
        }
        bt = p;
        bt->parent = NULL;
    }
}

/**
 * 与左兄弟合并
 *
 * @param leftBrother  左兄弟结点
 * @param parent       双亲结点
 * @param p            B树结点
 * @param bt           B树
 * @param order        关键字位序
 */
void mergeWithLeftBrother(BTree &leftBrother, BTree &parent,
                          BTree &p, BTree &bt, int &order) {
// 与左兄弟合并
    leftBrother->key[leftBrother->keyNum + 1] = parent->key[order];    // 从父结点拿下分割本节点与左兄弟的关键字
    leftBrother->child[leftBrother->keyNum + 1] = p->child[0];
    if (leftBrother->child[leftBrother->keyNum + 1] != NULL) {
        leftBrother->child[leftBrother->keyNum +
                           1]->parent = leftBrother;    // 给左兄弟的结点，当此结点存在时需要把其父亲指向指向左结点
    }
    leftBrother->keyNum++; //左兄弟关键数加1
    for (int j = 1; j <= p->keyNum; j++) {
        // 把本结点的关键字和子树指针赋给左兄弟
        leftBrother->key[leftBrother->keyNum + j] = p->key[j];
        leftBrother->child[leftBrother->keyNum + j] = p->child[j];
        if (leftBrother->child[leftBrother->keyNum + j] != NULL) {
            leftBrother->child[leftBrother->keyNum + j]->parent = leftBrother;
        }
    }
    leftBrother->keyNum += p->keyNum;
    parent->child[order] = NULL;
    free(p);    // 释放p结点
    for (int j = order; j < parent->keyNum; j++) {
        // 左移
        parent->key[j] = parent->key[j + 1];
        parent->child[j] = parent->child[j + 1];
    }
    parent->child[parent->keyNum] = NULL;
    parent->keyNum--;        // 父结点关键字个数减1
    if (bt == parent) {
        // 如果此时父结点为根，则当父结点没有关键字时才调整
        if (0 == parent->keyNum) {
            for (int j = 0; j <= parent->keyNum + 1; j++) {
                if (parent->child[j] != NULL) {
                    bt = parent->child[j];
                    break;
                }
                bt->parent = NULL;
            }
        }
    } else {
        // 如果父结点不为根，则需要判断是否需要重新调整
        if (parent->keyNum < (STAGE - 1) / 2) {
            restore(bt, parent);
        }
    }
}

/**
 * 与右兄弟合并
 * @param rightBrother  右兄弟节点
 * @param parent        双亲节点
 * @param p             B树节点
 * @param bt            B树
 * @param order         关键字位序
 */
void mergeWithRightBrother(BTree &rightBrother, BTree &parent,
                           BTree &p, BTree &bt, int &order) {
// 与右兄弟合并
    for (int j = (rightBrother->keyNum); j > 0; j--) {
        if (j > 0) {
            rightBrother->key[j + 1 + p->keyNum] = rightBrother->key[j];
        }
        rightBrother->child[j + 1 + p->keyNum] = rightBrother->child[j];
    }
    rightBrother->key[p->keyNum + 1] = parent->key[order + 1];    // 把父结点的分割两个本兄弟和右兄弟的关键字拿下来使用
    for (int j = 0; j <= p->keyNum; j++) {
        // 把本结点的关键字及子树指针移动右兄弟中去
        if (j > 0) {
            rightBrother->key[j] = p->key[j];
        }
        rightBrother->child[j] = p->child[j];
        if (rightBrother->child[j] != NULL) {
            rightBrother->child[j]->parent = rightBrother;    // 给右兄弟的结点需要把其父结点指向右兄弟
        }
    }
    rightBrother->keyNum += (p->keyNum + 1);
    parent->child[order] = NULL;
    free(p); // 释放p结点
    for (int j = order; j < parent->keyNum; j++) {
        if (j > order) {
            parent->key[j] = parent->key[j + 1];
        }
        parent->child[j] = parent->child[j + 1];
    }
    if (1 == parent->keyNum) {
        // 如果父结点在关键字减少之前只有一个结点，那么需要把父结点的右孩子赋值给左孩子
        parent->child[0] = parent->child[1];
    }
    parent->child[parent->keyNum] = NULL;
    parent->keyNum--;                    // 父结点关键字数减1
    if (bt == parent) {
        //如果此时父结点为根，则当父结点没有关键字时才调整
        if (0 == parent->keyNum) {
            for (int j = 0; j <= parent->keyNum + 1; j++) {
                if (parent->child[j] != NULL) {
                    bt = parent->child[j];
                    break;
                }
            }
            bt->parent = NULL;
        }
    } else {
        //如果父结点不为根，则需要判断是否需要重新调整
        if (parent->keyNum < (STAGE - 1) / 2) {
            restore(bt, parent);
        }
    }
}

void deleteBTree(BTree &bt, KeyType key) {
    Result rs = static_cast<Result>(malloc(sizeof(BTNode)));
    searchBTree(bt, key, rs);
    if (!rs->tag) {
        return;
    }
    BTree p = rs->ptr;
    int order = rs->order;
    if (p->child[order] != NULL) {
        successor(p, order);
        deleteBTree(p, 0);
    } else {
        remove(p, order);
        if (p->keyNum < (STAGE - 1) / 2) {
            restore(bt, p);
        }
    }
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