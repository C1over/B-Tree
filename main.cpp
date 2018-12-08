#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <cctype>
#include "bTree.h"

#define IS_INITED 1
#define NO_INITED 2

void searchOperation(BTree bt, int status);

void showBTreeMenu() {
    printf("-----------------------------B树功能选择菜单---------------------------------\n");
    printf("                                                                           \n");
    printf("                          1. 创建一棵空B树                                   \n");
    printf("                          2. 插入关键字                                     \n");
    printf("                          3. 删除关键字                                     \n");
    printf("                          4. 查找关键字                                     \n");
    printf("                          5. 打印B树                                       \n");
    printf("                          6. 销毁B树                                       \n");
    printf("                          0. 退出                                          \n");
    printf("                                                                          \n");
    printf("--------------------------------------------------------------------------\n");
}

void initOperate(BTree bt, int &status) {
    if (status == IS_INITED) {
        printf("B树已经初始化！\n");
        return;
    }
    bt = static_cast<BTree>(malloc(sizeof(BTNode)));
    initialize(bt);
    status = IS_INITED;
    printf("成功初始化B树\n");
}

void insertOperate(BTree bt, int status) {
    KeyType key;
    if (status == NO_INITED) {
        printf("B树还未初始化，请先初始化！\n");
        return;
    }
    if (NULL == bt) {
        printf("当前B树是空的");
    } else {
        printfBTree(bt);
    }
    while (true) {
        printf("请输入您想插入的关键字：");
        scanf("%d", &key);
        bool r = insertBTree(bt, key);
        if (!r) {
            printf("该关键字已经存在于B树中\n");
        } else if (r) {
            printf("插入成功，插入后的B树如下：\n");
            printf("------------------------------------------------------\n");
            printfBTree(bt);
            printf("\n----------------------------------------------------\n");
        }
        printf("是否继续插入？（Y/y：是, 其他任意键为否）：");
        char c;
        getchar();
        scanf("%c", &c);
        if (toupper(c) != 'Y') {
            break;
        }
        printf("\n");
    }
}

void deleteOperation(BTree &bt, int status) {
    KeyType key;
    if (NO_INITED == status) {
        printf("B树还未初始化，请先初始化！\n");
        return;
    }
    if (NULL == bt) {
        printf("现在B树是空的，无法进行删除操作！\n");
        return;
    } else {
        printf("------------------------------------------------------\n");
        printfBTree(bt);
        printf("\n------------------------------------------------------\n");
        while (true) {
            printf("请输入您想删除的关键字：");
            while (1 != scanf("%d", &key)) {
                getchar();
                fflush(stdin);
                printf("请输入正确的值！\n");
                printf("请输入您想删除的关键字：");
            }
            bool r = deleteBTree(bt, key);
            if (r) {
                printf("删除成功，删除后的B树如下：\n");
                printf("------------------------------------------------------\n");
                printfBTree(bt);
                printf("\n------------------------------------------------------\n");
            } else {
                printf("该关键字不在B树中，无法删除！\n");
            }
            printf("是否继续删除？（Y/y：是, 其他任意键为否）：");
            char c;
            getchar();
            scanf("%c", &c);
            fflush(stdin);
            if (toupper(c) != 'Y') {
                break;
            }
            printf("\n");
        }

    }
}

void searchOperation(BTree bt, int status) {
    KeyType key;
    Result r;
    if (NO_INITED == status) {
        printf("B树还未初始化，请先初始化！\n");
        return;
    }
    if (NULL == bt) {
        printf("现在B树是空的，无法进行查找操作！\n");
        return;
    } else {
        printf("------------------------------------------------------\n");
        printfBTree(bt);
        printf("\n------------------------------------------------------\n");
        printf("请输入您要查找的关键字：");
        scanf("%d", &key);
        searchBTree(bt, key, r);
        if (!r->tag) {
            printf("没有查到该关键字。\n");
        } else {
            printf("以关键字所在结点的子树如下：\n");
            printf("------------------------------------------------------\n");
            printfBTree(r->ptr);
            printf("\n------------------------------------------------------\n");
        }
    }
}

void printfOperation(BTree bt, int status) {
    if (NO_INITED == status) {
        printf("B树还没初始化！请先初始化\n");
    } else if (NULL == bt) {
        printf("B树为空\n");
    } else {
        printf("------------------------------------------------------\n");
        printfBTree(bt);
        printf("\n------------------------------------------------------\n");
    }
}

void destroyOperation(BTree bt, int &status) {
    if (status == NO_INITED) {
        printf("B树还没创建，不用销毁\n");
    } else if (bt == NULL) {
        printf("B树为空，不用销毁\n");
    } else {
        printf("是否继续删除？（Y/y：是, 其他任意键为否）：");
        char c;
        getchar();
        scanf("%c", &c);
        fflush(stdin);
        if (toupper(c) != 'Y') {
            destroyBTree(bt);
            status = NO_INITED;
            printf("销毁成功\n");
        }
    }
}

int main() {
    int selection;
    BTree bt;
    int bTreeStatus = NO_INITED;
    while (true) {
        showBTreeMenu();
        printf("请输入您的选择：");
        scanf("%d", &selection);
        getchar();
        if (selection == 0) {
            printf("\n是否退出？（Y/y：是, 其他任意键为否）");
            char option = static_cast<char>(getchar());
            if (toupper(option) == 'Y') {
                if (bTreeStatus == IS_INITED) {
                    destroyBTree(bt);
                }
                exit(EXIT_SUCCESS);
            }
        } else {
            switch (selection) {
                case 1:
                    initOperate(bt, bTreeStatus);
                    break;
                case 2:
                    insertOperate(bt, bTreeStatus);
                    break;
                case 3:
                    deleteOperation(bt, bTreeStatus);
                    break;
                case 4:
                    searchOperation(bt, bTreeStatus);
                    break;
                case 5:
                    printfOperation(bt, bTreeStatus);
                    break;
                case 6:
                    destroyOperation(bt, bTreeStatus);
                    break;
                default:
                    printf("输入有误，请重试：\n");
                    break;
            }
        }
    }
}

