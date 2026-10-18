#include "binary_tree_internal.h"
#include "allocator.h"


static IntBinaryTreeNode *create_node(int value)
{
    IntBinaryTreeNode *node = alloc_mem(sizeof(IntBinaryTreeNode));

    if (node != NULL)
    {
        node->payload = value;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

static void fill_sorted_values(IntBinaryTreeNode *node, int *values, size_t *index)
{
    if (node == NULL)
    {
        return;
    }

    fill_sorted_values(node->left, values, index);
    values[*index] = node->payload;
    (*index)++;
    fill_sorted_values(node->right, values, index);
}

IntBinaryTree intbintree_new()
{
    IntBinaryTree tree = alloc_mem(sizeof(struct IntBinaryTreeRootNode));

    if (tree != NULL)
    {
        tree->root = NULL;
        tree->size = 0;
    }

    return tree;
}

void intbintree_insert(IntBinaryTree t, int value)
{
    IntBinaryTreeNode *node;
    IntBinaryTreeNode *current;

    if (t == NULL)
    {
        return;
    }

    node = create_node(value);
    if (node == NULL)
    {
        return;
    }

    if (t->root == NULL)
    {
        t->root = node;
        t->size++;
        return;
    }

    current = t->root;
    while (1)
    {
        if (value <= current->payload)
        {
            if (current->left == NULL)
            {
                current->left = node;
                t->size++;
                return;
            }

            current = current->left;
        }
        else
        {
            if (current->right == NULL)
            {
                current->right = node;
                t->size++;
                return;
            }

            current = current->right;
        }
    }
}

//TODO 6 use post-order traversal to free all nodes in the tree
//don't forget to free the tree struct itself after freeing all the nodes
void intbintree_free(IntBinaryTree t)
{
    if (t == NULL)
    {
        return;
    }

    free_node_post_order(t->root);

    free_mem(t);
}

void free_node_post_order(IntBinaryTreeNode *n)
{
    if (n == NULL)
        return;

    free_node_post_order(n->left);
    free_node_post_order(n->right);

    free_mem(n);
}

int intbintree_contains(IntBinaryTree t, int value)
{
    IntBinaryTreeNode *current;

    if (t == NULL)
    {
        return 0;
    }

    current = t->root;
    while (current != NULL)
    {
        if (value == current->payload)
        {
            return 1;
        }

        if (value <= current->payload)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return 0;
}

int *intbintree_to_sorted_array(IntBinaryTree t, size_t *out_size)
{
    int *values;
    size_t index = 0;

    if (out_size != NULL)
    {
        *out_size = 0;
    }

    if (t == NULL || out_size == NULL)
    {
        return NULL;
    }

    *out_size = t->size;
    if (t->size == 0)
    {
        return NULL;
    }

    values = alloc_mem(t->size * sizeof(int));
    if (values == NULL)
    {
        *out_size = 0;
        return NULL;
    }

    fill_sorted_values(t->root, values, &index);
    return values;
}

/////////////////////////////////////////////////
//remove
/////////////////////////////////////////////////
static void remove_leaf_node(IntBinaryTree t,
                             IntBinaryTreeNode *parent,
                             IntBinaryTreeNode *current)
{
    if (parent == NULL)
    {
        t->root = NULL;
    }
    else if (parent->left == current)
    {
        parent->left = NULL;
    }
    else
    {
        parent->right = NULL;
    }

    free_mem(current);
}

static void remove_single_child_node(IntBinaryTree t,
                                     IntBinaryTreeNode *parent,
                                     IntBinaryTreeNode *current)
{
    IntBinaryTreeNode *child = (current->left != NULL) ? current->left : current->right;

    if (parent == NULL)
    {
        t->root = child;
    }
    else if (parent->left == current)
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }

    free_mem(current);
}

static void remove_two_children_node(IntBinaryTreeNode *current)
{
    IntBinaryTreeNode *successorParent = current;
    IntBinaryTreeNode *successor = current->right;

    while (successor->left != NULL)
    {
        successorParent = successor;
        successor = successor->left;
    }

    current->payload = successor->payload;

    if (successorParent->left == successor)
    {
        successorParent->left = successor->right;
    }
    else
    {
        successorParent->right = successor->right;
    }

    free_mem(successor);
}

static void find_node_with_parent(IntBinaryTree t,
                                  int value,
                                  IntBinaryTreeNode **parent,
                                  IntBinaryTreeNode **current)
{
    //use local pointers to keep syntax cleaner
    IntBinaryTreeNode *p = NULL;
    IntBinaryTreeNode *c = NULL;

    //initialize output parameters to indicate not found
    *parent = NULL;
    *current = NULL;

    if (t == NULL)
    {
        return;
    }

    c = t->root;
    while (c != NULL)
    {
        if (value == c->payload)
        {
            //we found the node, set the output parameters and return
            *parent = p;
            *current = c;
            return;
        }

        p = c;
        if (value <= c->payload)
        {
            c = c->left;
        }
        else
        {
            c = c->right;
        }
    }
}

void intbintree_remove(IntBinaryTree t, int value)
{
    IntBinaryTreeNode *current;
    IntBinaryTreeNode *parent;

    if (t == NULL || t->root == NULL)
    {
        return;
    }

    find_node_with_parent(t, value, &parent, &current);
    if (current == NULL)
    {
        return;
    }

    if (current->left == NULL && current->right == NULL)
    {
        remove_leaf_node(t, parent, current);
    }
    else if (current->left == NULL || current->right == NULL)
    {
        remove_single_child_node(t, parent, current);
    }
    else
    {
        remove_two_children_node(current);
    }

    t->size--;
}
