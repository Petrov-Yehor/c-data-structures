extern "C" {
    #include "allocator.h"
    #include "binary_tree.h"
}

#include <gtest/gtest.h>
#include <limits>
#include <vector>

std::vector<int> collect_sorted_values(IntBinaryTree tree);

namespace api_tests {
class IntBinaryTreeApiTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mem_reset_stat();
        mem_block_allocs(false);
    }
};

}  // namespace api_tests

using api_tests::IntBinaryTreeApiTest;

TEST_F(IntBinaryTreeApiTest, NewTreeStartsEmptyViaPublicInterface)
{
    IntBinaryTree tree = intbintree_new();

    ASSERT_NE(tree, nullptr);
    EXPECT_TRUE(collect_sorted_values(tree).empty());
}

TEST_F(IntBinaryTreeApiTest, NewTreeReturnsNullWhenAllocationFails)
{
    mem_block_allocs(true);

    IntBinaryTree tree = intbintree_new();

    EXPECT_EQ(tree, nullptr);
}

TEST_F(IntBinaryTreeApiTest, FailedInsertLeavesTraversalUnchanged)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);
    std::vector<int> before_failure = collect_sorted_values(tree);

    mem_block_allocs(true);
    intbintree_insert(tree, 7);
    mem_block_allocs(false);

    EXPECT_EQ(collect_sorted_values(tree), before_failure);
}

TEST_F(IntBinaryTreeApiTest, ContainsAndRemoveReflectTreeContents)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {8, 3, 10, 1, 6, 14, 4, 7, 13}) {
        intbintree_insert(tree, value);
    }

    EXPECT_TRUE(intbintree_contains(tree, 6));
    EXPECT_TRUE(intbintree_contains(tree, 14));
    EXPECT_FALSE(intbintree_contains(tree, 2));

    intbintree_remove(tree, 1);
    EXPECT_EQ(collect_sorted_values(tree),
              (std::vector<int>{3, 4, 6, 7, 8, 10, 13, 14}));

    intbintree_remove(tree, 14);
    EXPECT_EQ(collect_sorted_values(tree),
              (std::vector<int>{3, 4, 6, 7, 8, 10, 13}));

    intbintree_remove(tree, 8);
    EXPECT_FALSE(intbintree_contains(tree, 8));
    EXPECT_EQ(collect_sorted_values(tree),
              (std::vector<int>{3, 4, 6, 7, 10, 13}));
}

TEST_F(IntBinaryTreeApiTest, SortedArrayReturnsOrderedCopyAndSize)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {5, 2, 8, 1, 3}) {
        intbintree_insert(tree, value);
    }

    size_t size = 99;
    int* values = intbintree_to_sorted_array(tree, &size);

    ASSERT_NE(values, nullptr);
    EXPECT_EQ(size, 5U);
    EXPECT_EQ(std::vector<int>(values, values + size), (std::vector<int>{1, 2, 3, 5, 8}));

    free_mem(values);
}

TEST_F(IntBinaryTreeApiTest, SortedArrayOnEmptyTreeReturnsNullAndZeroSize)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    size_t size = 99;
    int* values = intbintree_to_sorted_array(tree, &size);

    EXPECT_EQ(values, nullptr);
    EXPECT_EQ(size, 0U);
}

TEST_F(IntBinaryTreeApiTest, SortedArrayReturnsNullWhenAllocationFails)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 1);
    intbintree_insert(tree, 9);

    size_t size = 99;
    mem_block_allocs(true);
    int* values = intbintree_to_sorted_array(tree, &size);
    mem_block_allocs(false);

    EXPECT_EQ(values, nullptr);
    EXPECT_EQ(size, 0U);
    EXPECT_EQ(collect_sorted_values(tree), (std::vector<int>{1, 5, 9}));
}

TEST_F(IntBinaryTreeApiTest, FreeReleasesTreeAndAllNodes)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);

    EXPECT_EQ(mem_get_allocated_block_cnt(), 4);

    intbintree_free(tree);

    EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
    EXPECT_EQ(mem_get_free_call_cnt(), 4);
}
