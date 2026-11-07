/*
 * 章节：平衡二叉树（AVL 树）
 * 知识点：
 *   二叉搜索树最怕“按顺序插入”：1、2、3、4 会让树退化成一条链，查找变成 O(n)。
 *   AVL 树的办法是：每个结点记录左右子树的高度差，这个差叫平衡因子。
 *   平衡因子 = 左子树高度 - 右子树高度，只允许是 -1、0、1。
 *   插入后沿路径往回检查，一旦某个结点不平衡，就用旋转把它掰回来。
 *   四种不平衡和对应的旋转：
 *     LL（左边高，且插在左孩子的左边）-> 右旋一次
 *     RR（右边高，且插在右孩子的右边）-> 左旋一次
 *     LR（左边高，但插在左孩子的右边）-> 先对左孩子左旋，再对自己右旋
 *     RL（右边高，但插在右孩子的左边）-> 先对右孩子右旋，再对自己左旋
 *   旋转只改几个指针，不移动元素，一次旋转 O(1)。
 *   因为树始终平衡，高度是 O(log n)，查找、插入、删除都是 O(log n)。
 *   代价是每次插入/删除都要维护高度并可能旋转，写起来比普通 BST 复杂。
 *   应用：需要大量查找且要求有序的场合、内存中的有序映射、数据库索引的思想来源。
 *   实际工程里更常见的是红黑树（旋转更少，C++ map、Java TreeMap 都用它）。
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct AvlNode {
    int data;
    int height;
    struct AvlNode *left;
    struct AvlNode *right;
} AvlNode;

static int max_int(int a, int b)
{
    return a > b ? a : b;
}

static int height_of(const AvlNode *node)
{
    return node == NULL ? 0 : node->height;
}

static void update_height(AvlNode *node)
{
    node->height = 1 + max_int(height_of(node->left), height_of(node->right));
}

static int balance_factor(const AvlNode *node)
{
    return height_of(node->left) - height_of(node->right);
}

static AvlNode *create_node(int data)
{
    AvlNode *node = malloc(sizeof *node);
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* 右旋：把左孩子提上来
 *
 *        y                x
 *       / \              / \
 *      x   T3    -->    T1   y
 *     / \                  / \
 *    T1  T2               T2  T3
 */
static AvlNode *rotate_right(AvlNode *y)
{
    AvlNode *x = y->left;
    AvlNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    update_height(y);
    update_height(x);
    return x;
}

/* 左旋：把右孩子提上来，是右旋的镜像 */
static AvlNode *rotate_left(AvlNode *x)
{
    AvlNode *y = x->right;
    AvlNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    update_height(x);
    update_height(y);
    return y;
}

static AvlNode *avl_insert(AvlNode *node, int data)
{
    if (node == NULL) {
        return create_node(data);
    }

    if (data < node->data) {
        node->left = avl_insert(node->left, data);
    } else if (data > node->data) {
        node->right = avl_insert(node->right, data);
    } else {
        return node;      /* 重复值不插入 */
    }

    update_height(node);

    int bf = balance_factor(node);

    /* LL：左边高，且新结点在左孩子的左边 -> 右旋 */
    if (bf > 1 && data < node->left->data) {
        return rotate_right(node);
    }
    /* RR：右边高，且新结点在右孩子的右边 -> 左旋 */
    if (bf < -1 && data > node->right->data) {
        return rotate_left(node);
    }
    /* LR：左边高，但新结点在左孩子的右边 -> 先左旋左孩子，再右旋自己 */
    if (bf > 1 && data > node->left->data) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    /* RL：右边高，但新结点在右孩子的左边 -> 先右旋右孩子，再左旋自己 */
    if (bf < -1 && data < node->right->data) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

static void inorder(const AvlNode *root)
{
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

/* 前序遍历，顺便把每个结点的高度打出来，方便观察平衡效果 */
static void print_with_height(const AvlNode *root)
{
    if (root == NULL) {
        return;
    }
    printf("%d(h%d) ", root->data, root->height);
    print_with_height(root->left);
    print_with_height(root->right);
}

static void destroy_tree(AvlNode *root)
{
    if (root == NULL) {
        return;
    }
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

int main(void)
{
    AvlNode *root = NULL;

    /* 故意按从小到大的顺序插入：普通 BST 会变成一条链，高度是 10 */
    for (int i = 1; i <= 10; i++) {
        root = avl_insert(root, i);
    }

    printf("按 1~10 顺序插入 AVL 树\n");
    printf("中序遍历（有序）: ");
    inorder(root);
    printf("\n");

    printf("前序 + 高度: ");
    print_with_height(root);
    printf("\n");

    printf("树根是 %d，整棵树高度是 %d（普通 BST 会是 10）\n",
           root->data, root->height);
    printf("根结点的平衡因子: %d（只可能是 -1、0、1）\n",
           balance_factor(root));

    destroy_tree(root);
    printf("释放完成\n");
    return 0;
}
