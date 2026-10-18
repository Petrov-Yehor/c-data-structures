#ifndef _BINARY_TREE_INTERNAL_H
#define _BINARY_TREE_INTERNAL_H

#include "binary_tree.h"

typedef struct IntBinTreeNode
{
    int payload;
    struct IntBinTreeNode *left;
    struct IntBinTreeNode *right;
} IntBinaryTreeNode;

struct IntBinaryTreeRootNode
{
    IntBinaryTreeNode *root;
    size_t size;
};

#endif