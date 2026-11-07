/*
 * 章节：二叉搜索树（BST）
 * 知识点：
 *   二叉搜索树（Binary Search Tree）在普通二叉树上加一条规则：
 *     左子树里所有值 < 根的值 < 右子树里所有值。
 *   这条规则让查找像猜数字：比根小就往左走，比根大就往右走，每走一步排除一半。
 *   所以平均时间复杂度是 O(log n)，和二分查找一样。
 *   中序遍历一定是从小到大，因为顺序是“左 -> 根 -> 右”。
 *   插入就是一路比较，走到空位置挂上新结点。
 *   删除分三种情况：
 *     1. 叶子：直接删。
 *     2. 只有一个孩子：让孩子顶替自己的位置。
 *     3. 有两个孩子：找右子树里最小的结点（后继），把它的值复制过来，再删掉那个后继。
 *   最坏情况：按 1、2、3、4…… 的顺序插入，树会退化成一条链，所有操作变成 O(n)。
 *   所以真正的库会用 AVL 树或红黑树来保持平衡（下一章）。
 *   应用：有序集合、范围查询、数据库索引（B+ 树是它的多叉版本）、C++ 的 std::map/set。
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

static TreeNode *bst_insert(TreeNode *root, int data)
{
    if (root == NULL) {
        return create_node(data);
    }
    if (data < root->data) {
        root->left = bst_insert(root->left, data);
    } else if (data > root->data) {
        root->right = bst_insert(root->right, data);
    }
    /* 相等就不插入，保持集合里没有重复值 */
    return root;
}

static bool bst_search(const TreeNode *root, int key)
{
    while (root != NULL) {
        if (key == root->data) {
            return true;
        }
        root = (key < root->data) ? root->left : root->right;
    }
    return false;
}

static TreeNode *find_min(TreeNode *root)
{
    while (root != NULL && root->left != NULL) {
        root = root->left;
    }
    return root;
}

static TreeNode *bst_delete(TreeNode *root, int key)
{
    if (root == NULL) {
        return NULL;
    }

    if (key < root->data) {
        root->left = bst_delete(root->left, key);
    } else if (key > root->data) {
        root->right = bst_delete(root->right, key);
    } else {
        /* 找到了要删的结点 */
        if (root->left == NULL) {
            TreeNode *right = root->right;
            free(root);
            return right;
        }
        if (root->right == NULL) {
            TreeNode *left = root->left;
            free(root);
            return left;
        }

        /* 两个孩子：用右子树的最小值顶替 */
        TreeNode *successor = find_min(root->right);
        root->data = successor->data;
        root->right = bst_delete(root->right, successor->data);
    }
    return root;
}

static void inorder(const TreeNode *root)
{
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
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
    TreeNode *root = NULL;
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};

    for (size_t i = 0; i < sizeof values / sizeof values[0]; i++) {
        root = bst_insert(root, values[i]);
    }

    printf("插入 9 个数后，中序遍历（一定是从小到大）: ");
    inorder(root);
    printf("\n");

    printf("查找 6: %s\n", bst_search(root, 6) ? "找到" : "没找到");
    printf("查找 9: %s\n", bst_search(root, 9) ? "找到" : "没找到");
    printf("树的高度: %zu\n", tree_height(root));

    /* 情况 1：删除叶子结点 1 */
    root = bst_delete(root, 1);
    printf("\n删除叶子 1 之后: ");
    inorder(root);
    printf("\n");

    /* 情况 2：删除只有一个孩子的结点 10（它只有右孩子 14） */
    root = bst_delete(root, 10);
    printf("删除只有一个孩子的 10 之后: ");
    inorder(root);
    printf("\n");

    /* 情况 3：删除有两个孩子的根 8，用右子树最小值 13 顶替 */
    root = bst_delete(root, 8);
    printf("删除有两个孩子的根 8 之后: ");
    inorder(root);
    printf("\n高度变成 %zu\n", tree_height(root));

    destroy_tree(root);
    printf("释放完成\n");
    return 0;
}
