#include <stdbool.h>

#ifndef B_TREE_BTREE_H
#define B_TREE_BTREE_H
#define STAGE 3   // B树的阶
#endif //B_TREE_BTREE_H
typedef int KeyType;

typedef struct BTNode {
    int keyNum;                       // 关键字数量
    KeyType key[STAGE + 1];           // 关键字数组
    struct BTNode *child[STAGE + 1];  // 孩子节点指针数组
    struct BTNode *parent;            // 双亲节点指针
} BTNode, *BTree;

typedef struct result {
    BTree ptr;    // 指向找到的节点
    int order;    // 1=<order<=STAGE，在节点中关键字位序
    bool tag;     // true：查找成功，false：查找失败
} *Result;

typedef struct QNode {          //链表和链表结点类型
    BTree data;                 //数据域
    struct QNode *next;         //指针域
} QNode, *Queue;

/**
 * 初始化B树
 *  @param bt  B树
 */
void initialize(BTree &bt);

/**
 *  在B树中查找关键字key，以result返回
 * @param bt      查找的B树
 * @param key     关键字
 * @param result  返回结果
 */
void searchBTree(BTree bt, KeyType key, Result result);

/**
 *  在B树中添加关键字key
 * @param bt    添加的B树
 * @param key   关键字
 * @return      操作是否成功
 */
bool insertBTree(BTree &bt, KeyType key);

/**
 *  删除B树中的关键key
 * @param bt    删除的B树
 * @param key   关键字
 */
bool deleteBTree(BTree &bt, KeyType key);

/**
 *  摧毁B树，释放内存
 * @param bt    摧毁的B树
 */
void destroyBTree(BTree &bt);

/**
 * 以凹入表的形式打印B树
 *
 * @param t B-树
 */
void printfBTree(Queue q, BTree bt);

/**
 *  更换B树中的key
 * @param bt      B树
 * @param oKey    原来的key
 * @param nKey    新的key
 */
void replaceBTree(BTree &bt, KeyType oKey, KeyType nKey);