extern "C" {
    #include "allocator.h"
    #include "binary_tree.h"
    #include "binary_tree_internal.h"
}

#include <gtest/gtest.h>
#include <limits>
#include <vector>

namespace traversal_tests {

class IntBinaryTreeTraversalTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mem_reset_stat();
        mem_block_allocs(false);
    }
};

struct TraversalRecord {
    std::vector<int> values;
    int count;
};

std::vector<int>* g_callback_values = nullptr;

void append_value_without_context(int value)
{
    g_callback_values->push_back(value);
}

void count_callback(void* context, int)
{
    int* counter = static_cast<int*>(context);
    (*counter)++;
}

void record_value_with_context(void* context, int value)
{
    TraversalRecord* record = static_cast<TraversalRecord*>(context);
    record->values.push_back(value);
    record->count++;
}

void append_value_with_context(void* context, int value)
{
    std::vector<int>* values = static_cast<std::vector<int>*>(context);
    values->push_back(value);
}

std::vector<int> collect_traversal_values(IntBinaryTree tree, IntBinTreeTraversalMode traversal)
{
    std::vector<int> values;

    intbintree_apply_with_context(tree, traversal, &values, append_value_with_context);

    return values;
}

}  // namespace traversal_tests

using traversal_tests::IntBinaryTreeTraversalTest;
using traversal_tests::TraversalRecord;
using traversal_tests::append_value_without_context;
using traversal_tests::collect_traversal_values;
using traversal_tests::count_callback;
using traversal_tests::g_callback_values;
using traversal_tests::record_value_with_context;

TEST_F(IntBinaryTreeTraversalTest, ApplyTraversesSingleInsertedValue)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 42);

    EXPECT_EQ(collect_traversal_values(tree, INTBINTREE_TRAVERSAL_IN_ORDER), (std::vector<int>{42}));
}

TEST_F(IntBinaryTreeTraversalTest, ApplyInvokesCallbackWithoutAdditionalAllocation)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);

    std::vector<int> values;
    int alloc_before = mem_get_alloc_call_cnt();
    int free_before = mem_get_free_call_cnt();

    g_callback_values = &values;
    intbintree_apply(tree, INTBINTREE_TRAVERSAL_PRE_ORDER, append_value_without_context);
    g_callback_values = nullptr;

    EXPECT_EQ(values, (std::vector<int>{5, 2, 9}));
    EXPECT_EQ(mem_get_alloc_call_cnt(), alloc_before);
    EXPECT_EQ(mem_get_free_call_cnt(), free_before);
}

TEST_F(IntBinaryTreeTraversalTest, TraversalOnEmptyTreeDoesNothing)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    TraversalRecord record{{}, 0};

    intbintree_apply_with_context(tree, INTBINTREE_TRAVERSAL_IN_ORDER, &record, record_value_with_context);

    EXPECT_TRUE(record.values.empty());
    EXPECT_EQ(record.count, 0);
}

TEST_F(IntBinaryTreeTraversalTest, ApplyWithContextTraversesAllModesOnNonTrivialTree)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {8, 3, 10, 1, 6, 14, 4, 7, 13}) {
        intbintree_insert(tree, value);
    }

    EXPECT_EQ(collect_traversal_values(tree, INTBINTREE_TRAVERSAL_IN_ORDER),
              (std::vector<int>{1, 3, 4, 6, 7, 8, 10, 13, 14}));
    EXPECT_EQ(collect_traversal_values(tree, INTBINTREE_TRAVERSAL_PRE_ORDER),
              (std::vector<int>{8, 3, 1, 6, 4, 7, 10, 14, 13}));
    EXPECT_EQ(collect_traversal_values(tree, INTBINTREE_TRAVERSAL_POST_ORDER),
              (std::vector<int>{1, 4, 7, 6, 3, 13, 14, 10, 8}));
}

TEST_F(IntBinaryTreeTraversalTest, TraversalTraversesHandcraftedTreeInAllModes)
{
    IntBinTreeNode node1{1, nullptr, nullptr};
    IntBinTreeNode node4{4, nullptr, nullptr};
    IntBinTreeNode node7{7, nullptr, nullptr};
    IntBinTreeNode node13{13, nullptr, nullptr};
    IntBinTreeNode node6{6, &node4, &node7};
    IntBinTreeNode node14{14, &node13, nullptr};
    IntBinTreeNode node3{3, &node1, &node6};
    IntBinTreeNode node10{10, nullptr, &node14};
    IntBinTreeNode node8{8, &node3, &node10};
    IntBinaryTreeRootNode tree{&node8, 9U};

    EXPECT_EQ(collect_traversal_values(&tree, INTBINTREE_TRAVERSAL_IN_ORDER),
              (std::vector<int>{1, 3, 4, 6, 7, 8, 10, 13, 14}));
    EXPECT_EQ(collect_traversal_values(&tree, INTBINTREE_TRAVERSAL_PRE_ORDER),
              (std::vector<int>{8, 3, 1, 6, 4, 7, 10, 14, 13}));
    EXPECT_EQ(collect_traversal_values(&tree, INTBINTREE_TRAVERSAL_POST_ORDER),
              (std::vector<int>{1, 4, 7, 6, 3, 13, 14, 10, 8}));
}

TEST_F(IntBinaryTreeTraversalTest, ApplyWithContextCanAccumulateState)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    for (int value : {5, 2, 8, 1, 3}) {
        intbintree_insert(tree, value);
    }

    int counter = 0;
    intbintree_apply_with_context(tree, INTBINTREE_TRAVERSAL_PRE_ORDER, &counter, count_callback);

    EXPECT_EQ(counter, 5);
}

TEST_F(IntBinaryTreeTraversalTest, ApplyWithContextVisitsEveryNodeExactlyOnce)
{
    IntBinTreeNode node1{1, nullptr, nullptr};
    IntBinTreeNode node4{4, nullptr, nullptr};
    IntBinTreeNode node7{7, nullptr, nullptr};
    IntBinTreeNode node13{13, nullptr, nullptr};
    IntBinTreeNode node6{6, &node4, &node7};
    IntBinTreeNode node14{14, &node13, nullptr};
    IntBinTreeNode node3{3, &node1, &node6};
    IntBinTreeNode node10{10, nullptr, &node14};
    IntBinTreeNode node8{8, &node3, &node10};
    IntBinaryTreeRootNode tree{&node8, 9U};
    TraversalRecord record{{}, 0};

    intbintree_apply_with_context(&tree, INTBINTREE_TRAVERSAL_IN_ORDER, &record, record_value_with_context);

    EXPECT_EQ(record.count, 9);
    EXPECT_EQ(record.values, (std::vector<int>{1, 3, 4, 6, 7, 8, 10, 13, 14}));
}

TEST_F(IntBinaryTreeTraversalTest, ApplyWithNullCallbackLeavesAllocationCountsUnchanged)
{
    IntBinaryTree tree = intbintree_new();
    ASSERT_NE(tree, nullptr);

    intbintree_insert(tree, 5);
    intbintree_insert(tree, 2);
    intbintree_insert(tree, 9);

    int alloc_before = mem_get_alloc_call_cnt();
    int free_before = mem_get_free_call_cnt();

    intbintree_apply(tree, INTBINTREE_TRAVERSAL_PRE_ORDER, nullptr);
    intbintree_apply_with_context(tree, INTBINTREE_TRAVERSAL_POST_ORDER, nullptr, nullptr);

    EXPECT_EQ(mem_get_alloc_call_cnt(), alloc_before);
    EXPECT_EQ(mem_get_free_call_cnt(), free_before);
}

TEST_F(IntBinaryTreeTraversalTest, InOrderTraversalHandlesExtremeIntegerValues)
{
    IntBinTreeNode min_node{std::numeric_limits<int>::min(), nullptr, nullptr};
    IntBinTreeNode max_node{std::numeric_limits<int>::max(), nullptr, nullptr};
    IntBinTreeNode root{0, &min_node, &max_node};
    IntBinaryTreeRootNode tree{&root, 3U};

    EXPECT_EQ(collect_traversal_values(&tree, INTBINTREE_TRAVERSAL_IN_ORDER),
              (std::vector<int>{std::numeric_limits<int>::min(), 0, std::numeric_limits<int>::max()}));
}