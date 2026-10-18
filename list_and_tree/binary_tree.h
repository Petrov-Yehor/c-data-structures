#ifndef _BINARY_TREE_H
#define _BINARY_TREE_H

#include <stddef.h>

typedef struct IntBinaryTreeRootNode* IntBinaryTree;

IntBinaryTree intbintree_new();

void intbintree_free(IntBinaryTree t);

void intbintree_insert(IntBinaryTree t, int value);

void intbintree_remove(IntBinaryTree t, int value);

int intbintree_contains(IntBinaryTree t, int value);

int* intbintree_to_sorted_array(IntBinaryTree t, size_t *out_size);


//tree traversal with higher order functions
typedef enum IntBinTreeTraversalMode
{
	INTBINTREE_TRAVERSAL_IN_ORDER,
	INTBINTREE_TRAVERSAL_PRE_ORDER,
	INTBINTREE_TRAVERSAL_POST_ORDER
} IntBinTreeTraversalMode;

typedef void (*IntBinTreeTraversalCallback)(int value);
typedef void (*IntBinTreeTraversalCallbackWithContext)(void *context, int value);

//traverses the tree in the specified order and calls the callback for each value
void intbintree_apply(IntBinaryTree t, IntBinTreeTraversalMode traversal, IntBinTreeTraversalCallback callback);

//traverses the tree in the specified order and calls the callback for each value, passing the context as well
void intbintree_apply_with_context(IntBinaryTree t, IntBinTreeTraversalMode traversal, void *context, IntBinTreeTraversalCallbackWithContext callback);

#endif