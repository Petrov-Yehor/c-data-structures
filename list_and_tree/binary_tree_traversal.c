#include "binary_tree_internal.h"

static void apply_callback(void *context,
                           int value,
                           IntBinTreeTraversalCallback callback,
                           IntBinTreeTraversalCallbackWithContext callback_with_context)
{
    if (callback_with_context != NULL)
    {
        callback_with_context(context, value);
    }
    else if (callback != NULL)
    {
        callback(value);
    }
}

//TODO 7 implement in order traversal using the helper function apply_callback to apply the callback to each node's payload in the correct order
static void apply_in_order(IntBinaryTreeNode *node,
                           void *context,
                           IntBinTreeTraversalCallback callback,
                           IntBinTreeTraversalCallbackWithContext callback_with_context)
{
    if (node == NULL)
    {
        return;
    }

    apply_in_order(node->left, context, callback, callback_with_context);
    apply_callback(context, node->payload, callback, callback_with_context);
    apply_in_order(node->right, context, callback, callback_with_context);
}

//TODO 7 implement pre order traversal using the helper function apply_callback to apply the callback to each node's payload in the correct order
static void apply_pre_order(IntBinaryTreeNode *node,
                            void *context,
                            IntBinTreeTraversalCallback callback,
                            IntBinTreeTraversalCallbackWithContext callback_with_context)
{
    if (node == NULL)
    {
        return;
    }

    apply_callback(context, node->payload, callback, callback_with_context);
    apply_pre_order(node->left, context, callback, callback_with_context);
    apply_pre_order(node->right, context, callback, callback_with_context);
}

//TODO 7 implement post order traversal using the helper function apply_callback to apply the callback to each node's payload in the correct order
static void apply_post_order(IntBinaryTreeNode *node,
                             void *context,
                             IntBinTreeTraversalCallback callback,
                             IntBinTreeTraversalCallbackWithContext callback_with_context)
{
    if (node == NULL)
    {
        return;
    }

    apply_post_order(node->left, context, callback, callback_with_context);
    apply_post_order(node->right, context, callback, callback_with_context);
    apply_callback(context, node->payload, callback, callback_with_context);
}

//TODO 7 choose the appropriate traversal function to apply based on the traversal parameter, and call it with the provided callback and context
static void apply_for_traversal(IntBinaryTreeNode *node,
                                IntBinTreeTraversalMode traversal,
                                void *context,
                                IntBinTreeTraversalCallback callback,
                                IntBinTreeTraversalCallbackWithContext callback_with_context)
{
    if (callback == NULL && callback_with_context == NULL)
    {
        return;
    }


    switch (traversal)
    {
        case INTBINTREE_TRAVERSAL_IN_ORDER:
            apply_in_order(node, context, callback, callback_with_context);
        break;
        case INTBINTREE_TRAVERSAL_POST_ORDER:
            apply_post_order(node, context, callback, callback_with_context);
        break;
        case INTBINTREE_TRAVERSAL_PRE_ORDER:
            apply_pre_order(node, context, callback, callback_with_context);
        break;
    }
}

//already implemented.
void intbintree_apply(IntBinaryTree t,
                      IntBinTreeTraversalMode traversal,
                      IntBinTreeTraversalCallback callback)
{
    if (t == NULL || callback == NULL)
    {
        return;
    }

    apply_for_traversal(t->root, traversal, NULL, callback, NULL);
}

void intbintree_apply_with_context(IntBinaryTree t,
                                   IntBinTreeTraversalMode traversal,
                                   void *context,
                                   IntBinTreeTraversalCallbackWithContext callback)
{
    if (t == NULL || callback == NULL)
    {
        return;
    }

    apply_for_traversal(t->root, traversal, context, NULL, callback);
}