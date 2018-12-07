//
// Created by Cc1over on 2018/12/6.
//

#include <stdbool.h>

#ifndef B_TREE_BTREE_H
#define B_TREE_BTREE_H
#define STAGE 3   // B树的阶
#endif //B_TREE_BTREE_H
typedef int KeyType;

typedef struct BTNode {
    int keyNum;                       // 关键字数量
    KeyType key[STAGE];               // 关键字数组
    struct BTNode *child[STAGE + 1];  // 孩子节点指针数组
} BTNode, *BTree;

typedef struct Result {
    BTree bt;    // 指向找到的节点
    int order;   // 1=<order<=STAGE，在节点中关键字位序
    int tag;     // 1：查找成功，0：查找失败
} Result;

/**
 *  初始化B树
 * @param bt   初始化的B树
 */
void initialize(BTree &bt);

/**
 *  在B树中查找关键字key，以result返回
 * @param bt      查找的B树
 * @param key     关键字
 * @param result  返回结果
 */
void searchBTree(BTree bt, KeyType key, Result *result);

/**
 *  在B树中添加关键字key
 * @param bt    添加的B树
 * @param key   关键字
 */
void insertBTree(BTree &bt, KeyType key);

/**
 *  删除B树中的关键key
 * @param bt    删除的B树
 * @param key   关键字
 */
void deleteBTree(BTree &bt, KeyType key);

/**
 *  摧毁B树，释放内存
 * @param bt    摧毁的B树
 */
void destroyBTree(BTree &bt);