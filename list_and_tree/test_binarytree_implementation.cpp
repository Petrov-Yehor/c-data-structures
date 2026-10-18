extern "C" {
    #include "allocator.h"
    #include "binary_tree.h"
    #include "binary_tree_internal.h"
}

#include <gtest/gtest.h>
#include <vector>

std::vector<int> collect_sorted_values(IntBinaryTree tree);

namespace implementation_tests {

class IntBinaryTreeTest : public ::testing::Test
{
 protected:
    void SetUp() override
    {
        mem_reset_stat();
        mem_block_allocs(false);
    }
};

}  // namespace implementation_tests

using implementation_tests::IntBinaryTreeTest;

TEST_F(IntBinaryTreeTest, NewTreeStartsEmpty)
{
    IntBinaryTree tree = intbintree_new();

    ASSERT_NE(tree, nullptr);
    EXPECT_EQ(tree->root, nullptr);
    EXPECT_EQ(tree->size, 0U);
}

TEST_F(IntBinaryTreeTest, NewTreeReturnsNullWhenAllocationFails)
{
    mem_block_allocs(true);

    IntBinaryTree tree = intbintree_new();

    EXPECT_EQ(tree, nullptr);
}

TEST_F(IntBinaryTreeTest, SingleInsertedValueBecomesRootNode)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 42);

    ASSERT_NE(tree->root, nullptr);
    EXPECT_EQ(tree->root->payload, 42);
    EXPECT_EQ(tree->root->left, nullptr);
    EXPECT_EQ(tree->root->right, nullptr);
    EXPECT_EQ(tree->size, 1U);
}

TEST_F(IntBinaryTreeTest, InsertPlacesSmallerValuesLeftAndLargerValuesRight)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {8, 3, 10, 1, 6}) {
        intbintree_insert(tree, value);
    }

    ASSERT_NE(tree->root, nullptr);
    ASSERT_NE(tree->root->left, nullptr);
    ASSERT_NE(tree->root->right, nullptr);
    EXPECT_EQ(tree->root->payload, 8);
    EXPECT_EQ(tree->root->left->payload, 3);
    EXPECT_EQ(tree->root->right->payload, 10);
    ASSERT_NE(tree->root->left->left, nullptr);
    ASSERT_NE(tree->root->left->right, nullptr);
    EXPECT_EQ(tree->root->left->left->payload, 1);
    EXPECT_EQ(tree->root->left->right->payload, 6);
    EXPECT_EQ(tree->size, 5U);
}

TEST_F(IntBinaryTreeTest, FailedInsertLeavesExistingTreeUnchanged)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);

    ASSERT_NE(tree->root, nullptr);
    ASSERT_NE(tree->root->left, nullptr);
    ASSERT_NE(tree->root->right, nullptr);
    IntBinTreeNode* original_root = tree->root;
    IntBinTreeNode* original_left = tree->root->left;
    IntBinTreeNode* original_right = tree->root->right;
    size_t original_size = tree->size;

    mem_block_allocs(true);
    intbintree_insert(tree, 7);

    EXPECT_EQ(tree->root, original_root);
    EXPECT_EQ(tree->root->left, original_left);
    EXPECT_EQ(tree->root->right, original_right);
    EXPECT_EQ(tree->size, original_size);
}

TEST_F(IntBinaryTreeTest, DuplicateValuesAreInsertedIntoLeftSubtree)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 5);
    intbintree_insert(tree, 5);

    ASSERT_NE(tree->root, nullptr);
    ASSERT_NE(tree->root->left, nullptr);
    ASSERT_NE(tree->root->left->left, nullptr);
    EXPECT_EQ(tree->root->payload, 5);
    EXPECT_EQ(tree->root->left->payload, 5);
    EXPECT_EQ(tree->root->left->left->payload, 5);
    EXPECT_EQ(tree->root->right, nullptr);
    EXPECT_EQ(tree->size, 3U);
}

TEST_F(IntBinaryTreeTest, RemoveLeafNodeClearsParentsChildPointer)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {8, 3, 10, 1, 6}) {
        intbintree_insert(tree, value);
    }

    ASSERT_NE(tree->root, nullptr);
    ASSERT_NE(tree->root->left, nullptr);
    ASSERT_NE(tree->root->left->left, nullptr);

    intbintree_remove(tree, 1);

    EXPECT_EQ(tree->root->payload, 8);
    ASSERT_NE(tree->root->left, nullptr);
    EXPECT_EQ(tree->root->left->payload, 3);
    EXPECT_EQ(tree->root->left->left, nullptr);
    ASSERT_NE(tree->root->left->right, nullptr);
    EXPECT_EQ(tree->root->left->right->payload, 6);
    EXPECT_EQ(tree->size, 4U);
}

TEST_F(IntBinaryTreeTest, RemoveOneChildNodePromotesItsChild)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {8, 3, 10, 14, 13}) {
        intbintree_insert(tree, value);
    }

    ASSERT_NE(tree->root, nullptr);
    ASSERT_NE(tree->root->right, nullptr);
    ASSERT_NE(tree->root->right->right, nullptr);
    ASSERT_NE(tree->root->right->right->left, nullptr);

    intbintree_remove(tree, 14);

    EXPECT_EQ(tree->root->payload, 8);
    ASSERT_NE(tree->root->right, nullptr);
    EXPECT_EQ(tree->root->right->payload, 10);
    EXPECT_EQ(tree->root->right->left, nullptr);
    ASSERT_NE(tree->root->right->right, nullptr);
    EXPECT_EQ(tree->root->right->right->payload, 13);
    EXPECT_EQ(tree->size, 4U);
}

TEST_F(IntBinaryTreeTest, RemoveTwoChildrenNodeReplacesPayloadWithSuccessor)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {8, 3, 10, 1, 6, 4, 7}) {
        intbintree_insert(tree, value);
    }

    ASSERT_NE(tree->root, nullptr);
    ASSERT_NE(tree->root->left, nullptr);
    ASSERT_NE(tree->root->left->left, nullptr);
    ASSERT_NE(tree->root->left->right, nullptr);

    intbintree_remove(tree, 3);

    EXPECT_EQ(tree->root->payload, 8);
    ASSERT_NE(tree->root->left, nullptr);
    EXPECT_EQ(tree->root->left->payload, 4);
    ASSERT_NE(tree->root->left->left, nullptr);
    EXPECT_EQ(tree->root->left->left->payload, 1);
    ASSERT_NE(tree->root->left->right, nullptr);
    EXPECT_EQ(tree->root->left->right->payload, 6);
    ASSERT_NE(tree->root->left->right->right, nullptr);
    EXPECT_EQ(tree->root->left->right->right->payload, 7);
    EXPECT_EQ(tree->size, 6U);
}

TEST_F(IntBinaryTreeTest, RemoveRootWithTwoChildrenUsesInOrderSuccessor)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {8, 3, 10, 1, 6, 14, 4, 7, 13}) {
        intbintree_insert(tree, value);
    }

    intbintree_remove(tree, 8);

    ASSERT_NE(tree->root, nullptr);
    EXPECT_EQ(tree->root->payload, 10);
    ASSERT_NE(tree->root->left, nullptr);
    ASSERT_NE(tree->root->right, nullptr);
    EXPECT_EQ(tree->root->left->payload, 3);
    EXPECT_EQ(tree->root->right->payload, 14);
    ASSERT_NE(tree->root->right->left, nullptr);
    EXPECT_EQ(tree->root->right->left->payload, 13);
    EXPECT_EQ(tree->size, 8U);
}

TEST_F(IntBinaryTreeTest, RemoveMissingValueLeavesStructureAndCountsUnchanged)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);

    IntBinTreeNode* original_root = tree->root;
    IntBinTreeNode* original_left = tree->root->left;
    IntBinTreeNode* original_right = tree->root->right;
    size_t original_size = tree->size;

    intbintree_remove(tree, 7);

    EXPECT_EQ(tree->root, original_root);
    EXPECT_EQ(tree->root->left, original_left);
    EXPECT_EQ(tree->root->right, original_right);
    EXPECT_EQ(tree->size, original_size);
    EXPECT_EQ(mem_get_free_call_cnt(), 0);
}

TEST_F(IntBinaryTreeTest, SortedArrayFailureDoesNotModifyTree)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);

    size_t size = 99;
    mem_block_allocs(true);
    int* values = intbintree_to_sorted_array(tree, &size);

    EXPECT_EQ(values, nullptr);
    EXPECT_EQ(size, 0U);
    ASSERT_NE(tree->root, nullptr);
    ASSERT_NE(tree->root->left, nullptr);
    ASSERT_NE(tree->root->right, nullptr);
    EXPECT_EQ(tree->root->payload, 5);
    EXPECT_EQ(tree->root->left->payload, 2);
    EXPECT_EQ(tree->root->right->payload, 9);
    EXPECT_EQ(tree->size, 3U);
}

TEST_F(IntBinaryTreeTest, FreeReleasesRootNodeAndTreeContainer)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);
    intbintree_insert(tree, 1);

    EXPECT_EQ(mem_get_allocated_block_cnt(), 5);

    intbintree_free(tree);

    EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
    EXPECT_EQ(mem_get_free_call_cnt(), 5);
}

