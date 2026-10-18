extern "C" {
    #include "allocator.h"
    #include "int_string_dictionary.h"
    #include "int_string_dictionary_internal.h"
}

#include <gtest/gtest.h>

namespace {

class IntStringDict : public ::testing::Test
{
protected:
    IntStringDictionary dictionary = nullptr;

    void SetUp() override
    {
        mem_reset_stat();
        mem_block_allocs(false);
    }

    void TearDown() override
    {
        if (dictionary != nullptr)
        {
            intstrdict_free(dictionary);
            dictionary = nullptr;
        }

        EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
    }
};

}  // namespace

TEST_F(IntStringDict, NewDictionaryStartsEmpty)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    EXPECT_EQ(dictionary->root, nullptr);
    EXPECT_EQ(dictionary->size, 0U);
    EXPECT_FALSE(intstrdict_contains(dictionary, 5));
    EXPECT_EQ(intstrdict_lookup(dictionary, 5), nullptr);
}

TEST_F(IntStringDict, NewDictionaryReturnsNullWhenAllocationFails)
{
    mem_block_allocs(true);

    dictionary = intstrdict_new();

    EXPECT_EQ(dictionary, nullptr);
    mem_block_allocs(false);
    EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
}

TEST_F(IntStringDict, InsertCopiesStringAndLookupReturnsStoredCopy)
{
    char value[] = "alpha";

    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 10, value);

    ASSERT_NE(dictionary->root, nullptr);
    ASSERT_NE(intstrdict_lookup(dictionary, 10), nullptr);
    EXPECT_STREQ(intstrdict_lookup(dictionary, 10), "alpha");
    EXPECT_NE(intstrdict_lookup(dictionary, 10), value);
    EXPECT_EQ(dictionary->size, 1U);

    value[0] = 'o';
    EXPECT_STREQ(intstrdict_lookup(dictionary, 10), "alpha");
}

TEST_F(IntStringDict, InsertOrdersNodesByIntegerKey)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 8, "root");
    intstrdict_insert(dictionary, 3, "left");
    intstrdict_insert(dictionary, 10, "right");
    intstrdict_insert(dictionary, 6, "inner");

    ASSERT_NE(dictionary->root, nullptr);
    ASSERT_NE(dictionary->root->left, nullptr);
    ASSERT_NE(dictionary->root->right, nullptr);
    EXPECT_EQ(dictionary->root->payload.key, 8);
    EXPECT_EQ(dictionary->root->left->payload.key, 3);
    EXPECT_EQ(dictionary->root->right->payload.key, 10);
    ASSERT_NE(dictionary->root->left->right, nullptr);
    EXPECT_EQ(dictionary->root->left->right->payload.key, 6);
}

TEST_F(IntStringDict, InsertExistingKeyReplacesStoredString)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 5, "first");
    char *first_lookup = intstrdict_lookup(dictionary, 5);

    intstrdict_insert(dictionary, 5, "second");

    ASSERT_NE(intstrdict_lookup(dictionary, 5), nullptr);
    EXPECT_STREQ(intstrdict_lookup(dictionary, 5), "second");
    EXPECT_NE(intstrdict_lookup(dictionary, 5), first_lookup);
    EXPECT_EQ(dictionary->size, 1U);
    EXPECT_EQ(mem_get_allocated_block_cnt(), 3);
}

TEST_F(IntStringDict, InsertDuplicateKeyFreesPreviouslyStoredString)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 5, "first");
    char *old_value = intstrdict_lookup(dictionary, 5);

    ASSERT_NE(old_value, nullptr);
    EXPECT_TRUE(mem_is_allocated(old_value));

    intstrdict_insert(dictionary, 5, "second");

    char *new_value = intstrdict_lookup(dictionary, 5);
    ASSERT_NE(new_value, nullptr);
    EXPECT_STREQ(new_value, "second");
    EXPECT_NE(new_value, old_value);
    EXPECT_FALSE(mem_is_allocated(old_value));
    EXPECT_TRUE(mem_is_allocated(new_value));
    EXPECT_EQ(mem_get_free_call_cnt(), 1);
    EXPECT_EQ(mem_get_allocated_block_cnt(), 3);
}

TEST_F(IntStringDict, FailedInsertLeavesExistingDictionaryUnchanged)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 5, "five");
    intstrdict_insert(dictionary, 2, "two");

    ASSERT_NE(dictionary->root, nullptr);
    ASSERT_NE(dictionary->root->left, nullptr);
    IntStringDictionaryNode *original_root = dictionary->root;
    IntStringDictionaryNode *original_left = dictionary->root->left;
    size_t original_size = dictionary->size;
    char *stored_value = intstrdict_lookup(dictionary, 2);

    mem_block_allocs(true);
    intstrdict_insert(dictionary, 7, "seven");
    mem_block_allocs(false);

    EXPECT_EQ(dictionary->root, original_root);
    EXPECT_EQ(dictionary->root->left, original_left);
    EXPECT_EQ(dictionary->size, original_size);
    EXPECT_EQ(intstrdict_lookup(dictionary, 2), stored_value);
    EXPECT_EQ(intstrdict_lookup(dictionary, 7), nullptr);
}

TEST_F(IntStringDict, FailedReplacementLeavesOldValueIntact)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 5, "five");

    mem_block_allocs(true);
    intstrdict_insert(dictionary, 5, "updated");
    mem_block_allocs(false);

    ASSERT_NE(intstrdict_lookup(dictionary, 5), nullptr);
    EXPECT_STREQ(intstrdict_lookup(dictionary, 5), "five");
    EXPECT_EQ(dictionary->size, 1U);
}

TEST_F(IntStringDict, RemoveLeafNodeFreesStoredString)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 8, "eight");
    intstrdict_insert(dictionary, 3, "three");
    intstrdict_insert(dictionary, 1, "one");

    EXPECT_EQ(mem_get_allocated_block_cnt(), 7);

    intstrdict_remove(dictionary, 1);

    EXPECT_EQ(intstrdict_lookup(dictionary, 1), nullptr);
    EXPECT_EQ(dictionary->size, 2U);
    EXPECT_EQ(mem_get_allocated_block_cnt(), 5);
}

TEST_F(IntStringDict, RemoveNodeWithTwoChildrenPromotesSuccessorPayload)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 8, "eight");
    intstrdict_insert(dictionary, 3, "three");
    intstrdict_insert(dictionary, 10, "ten");
    intstrdict_insert(dictionary, 1, "one");
    intstrdict_insert(dictionary, 6, "six");
    intstrdict_insert(dictionary, 4, "four");
    intstrdict_insert(dictionary, 7, "seven");

    intstrdict_remove(dictionary, 3);

    ASSERT_NE(dictionary->root, nullptr);
    ASSERT_NE(dictionary->root->left, nullptr);
    EXPECT_EQ(dictionary->root->left->payload.key, 4);
    EXPECT_STREQ(dictionary->root->left->payload.value, "four");
    EXPECT_EQ(intstrdict_lookup(dictionary, 3), nullptr);
    EXPECT_STREQ(intstrdict_lookup(dictionary, 4), "four");
    EXPECT_STREQ(intstrdict_lookup(dictionary, 6), "six");
    EXPECT_EQ(dictionary->size, 6U);
}

TEST_F(IntStringDict, ContainsLookupAndRemoveReflectDictionaryContents)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 12, "twelve");
    intstrdict_insert(dictionary, 4, "four");
    intstrdict_insert(dictionary, 20, "twenty");

    EXPECT_TRUE(intstrdict_contains(dictionary, 4));
    EXPECT_FALSE(intstrdict_contains(dictionary, 9));
    ASSERT_NE(intstrdict_lookup(dictionary, 20), nullptr);
    EXPECT_STREQ(intstrdict_lookup(dictionary, 20), "twenty");

    intstrdict_remove(dictionary, 20);

    EXPECT_FALSE(intstrdict_contains(dictionary, 20));
    EXPECT_EQ(intstrdict_lookup(dictionary, 20), nullptr);
    EXPECT_EQ(dictionary->size, 2U);
}

TEST_F(IntStringDict, FreeReleasesDictionaryNodesAndCopiedStrings)
{
    dictionary = intstrdict_new();
    ASSERT_NE(dictionary, nullptr);

    intstrdict_insert(dictionary, 5, "five");
    intstrdict_insert(dictionary, 2, "two");
    intstrdict_insert(dictionary, 9, "nine");

    EXPECT_EQ(mem_get_allocated_block_cnt(), 7);

    intstrdict_free(dictionary);
    dictionary = nullptr;

    EXPECT_EQ(mem_get_allocated_block_cnt(), 0);
    EXPECT_EQ(mem_get_free_call_cnt(), 7);
}