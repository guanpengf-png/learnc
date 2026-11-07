/*
 * 章节：二叉树与遍历
 * 知识点：
 *   二叉树每个结点最多两个子结点，分别叫左孩子和右孩子。
 *   层次从根开始算，根是第 1 层；第 i 层最多 2^(i-1) 个结点，深度 k 的树最多 2^k - 1 个结点。
 *   遍历就是把每个结点都访问一次。三种深度优先遍历只差“根”放在哪：
 *     前序：根 -> 左 -> 右       （复制一棵树、打印目录结构）
 *     中序：左 -> 根 -> 右       （二叉搜索树中序会得到从小到大）
 *     后序：左 -> 右 -> 根       （释放内存必须后序：先释放孩子再释放自己）
 *   层序遍历一层一层访问，需要队列帮忙（先进先出）。这里用数组模拟队列。
 *   递归写遍历非常短，因为“左子树、右子树”本身还是二叉树，天然适合递归。
 *   应用：表达式树（前中后缀表达式）、文件目录、决策树、堆（一种特殊的完全二叉树）、数据库索引。
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

static TreeNode *create_node(int data)
{
    TreeNode *node = malloc(sizeof *node);
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void preorder(const TreeNode *root)
{
    if (root == NULL) {
        return;
    }
    printf("%d ", root->data);   /* 根 */
    preorder(root->left);        /* 左 */
    preorder(root->right);       /* 右 */
}

static void inorder(const TreeNode *root)
{
    if (root == NULL) {
        return;
    }
    inorder(root->left);         /* 左 */
    printf("%d ", root->data);   /* 根 */
    inorder(root->right);        /* 右 */
}

static void postorder(const TreeNode *root)
{
    if (root == NULL) {
        return;
    }
    postorder(root->left);       /* 左 */
    postorder(root->right);      /* 右 */
    printf("%d ", root->data);   /* 根 */
}

/* 层序遍历：用数组当队列，head 出队，tail 入队 */
static void level_order(const TreeNode *root)
{
    if (root == NULL) {
        return;
    }

    const TreeNode *queue[64];
    size_t head = 0;
    size_t tail = 0;

    queue[tail++] = root;

    while (head < tail) {
        const TreeNode *node = queue[head++];
        printf("%d ", node->data);

        if (node->left != NULL) {
            queue[tail++] = node->left;
        }
        if (node->right != NULL) {
            queue[tail++] = node->right;
        }
    }
    printf("\n");
}

static size_t count_nodes(const TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

static size_t count_leaves(const TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    return count_leaves(root->left) + count_leaves(root->right);
}

static size_t tree_height(const TreeNode *root)
{
    if (root == NULL) {
        return 0;
    }
    size_t left = tree_height(root->left);
    size_t right = tree_height(root->right);
    return 1 + (left > right ? left : right);
}

/* 释放必须用后序：先释放左、右子树，最后释放自己 */
static void destroy_tree(TreeNode *root)
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
    /*
     *            8
     *          /   \
     *         3     10
     *        / \      \
     *       1   6      14
     *          / \    /
     *         4   7  13
     */
    TreeNode *root = create_node(8);
    if (root == NULL) {
        printf("内存不足\n");
        return 1;
    }
    root->left = create_node(3);
    root->right = create_node(10);
    root->left->left = create_node(1);
    root->left->right = create_node(6);
    root->left->right->left = create_node(4);
    root->left->right->right = create_node(7);
    root->right->right = create_node(14);
    root->right->right->left = create_node(13);

    printf("前序遍历（根左右）: ");
    preorder(root);
    printf("\n");

    printf("中序遍历（左根右）: ");
    inorder(root);
    printf("\n");

    printf("后序遍历（左右根）: ");
    postorder(root);
    printf("\n");

    printf("层序遍历（一层一层）: ");
    level_order(root);

    printf("结点总数 %zu\n", count_nodes(root));
    printf("叶子结点 %zu 个\n", count_leaves(root));
    printf("树的高度 %zu\n", tree_height(root));

    destroy_tree(root);
    printf("释放完成\n");
    return 0;
}
