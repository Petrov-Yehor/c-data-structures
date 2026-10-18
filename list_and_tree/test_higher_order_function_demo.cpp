#include <gtest/gtest.h>

#include <string>
#include <vector>

extern "C" {
#include "allocator.h"
#include "generic_list.h"
#include "generic_list_functions.h"
#include "higher_order_function_demo.h"
}

namespace {

class HofDemo : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mem_reset_stat();
        mem_block_allocs(false);
        hofdemo_reset_every_fifth_counter();
    }

    void TearDown() override
    {
        hofdemo_reset_every_fifth_counter();
    }
};

GenericList create_int_list(const std::vector<int> &values)
{
    GenericList list = genericlist_new();
    if (list == nullptr)
    {
        return nullptr;
    }

    for (int value : values)
    {
        genericlist_insert(list, &value, sizeof(int));
    }

    return list;
}

GenericList create_string_list(const std::vector<const char *> &values)
{
    GenericList list = genericlist_new();
    if (list == nullptr)
    {
        return nullptr;
    }

    for (const char *value : values)
    {
        genericlist_insert(list, (void *)value, strlen(value) + 1);
    }

    return list;
}

std::vector<int> read_int_list(GenericList list)
{
    std::vector<int> result;
    GenericListIterator it = genericlist_iterator_first(list);
    int *value = nullptr;

    while ((value = (int *)genericlist_iterator_value(it)) != nullptr)
    {
        result.push_back(*value);
        it = genericlist_iterator_move_next(it);
    }

    return result;
}

std::vector<std::string> read_string_list(GenericList list)
{
    std::vector<std::string> result;
    GenericListIterator it = genericlist_iterator_first(list);
    char *value = nullptr;

    while ((value = (char *)genericlist_iterator_value(it)) != nullptr)
    {
        result.push_back(value);
        it = genericlist_iterator_move_next(it);
    }

    return result;
}

GenericListGrouping *find_group(GenericList groups, const char *key)
{
    GenericListIterator it = genericlist_iterator_first(groups);
    GenericListGrouping *group = nullptr;

    while ((group = (GenericListGrouping *)genericlist_iterator_value(it)) != nullptr)
    {
        if (strcmp((char *)group->key, key) == 0)
        {
            return group;
        }
        it = genericlist_iterator_move_next(it);
    }

    return nullptr;
}

TEST_F(HofDemo, SumOfOddNumbersAggregatesOnlyOddValues)
{
    GenericList numbers = create_int_list({1, 2, 3, 4, 5, 6, 7});
    ASSERT_NE(numbers, nullptr);

    EXPECT_EQ(hofdemo_sum_of_odd_numbers(numbers), 16);

    genericlist_free(&numbers);
}

TEST_F(HofDemo, ExtractEveryFifthUsesModuleCounter)
{
    GenericList numbers = create_int_list({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
    ASSERT_NE(numbers, nullptr);

    GenericList result = hofdemo_extract_every_fifth(numbers);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(read_int_list(result), (std::vector<int>{5, 10}));

    genericlist_free(&result);
    genericlist_free(&numbers);
}

TEST_F(HofDemo, ResetFunctionRestartsEveryFifthSelection)
{
    GenericList numbers = create_int_list({1, 2, 3, 4, 5, 6});
    ASSERT_NE(numbers, nullptr);

    GenericList first_result = hofdemo_extract_every_fifth(numbers);
    ASSERT_NE(first_result, nullptr);
    EXPECT_EQ(read_int_list(first_result), (std::vector<int>{5}));
    genericlist_free(&first_result);

    GenericList second_result = hofdemo_extract_every_fifth(numbers);
    ASSERT_NE(second_result, nullptr);
    EXPECT_EQ(read_int_list(second_result), (std::vector<int>{4}));
    genericlist_free(&second_result);

    hofdemo_reset_every_fifth_counter();

    GenericList reset_result = hofdemo_extract_every_fifth(numbers);
    ASSERT_NE(reset_result, nullptr);
    EXPECT_EQ(read_int_list(reset_result), (std::vector<int>{5}));

    genericlist_free(&reset_result);
    genericlist_free(&numbers);
}

TEST_F(HofDemo, GroupWordsByInitialSeparatesVowelsAndConsonants)
{
    GenericList words = create_string_list({"apple", "banana", "orange", "pear", "umbrella"});
    ASSERT_NE(words, nullptr);

    GenericList groups = hofdemo_group_words_by_initial(words);

    ASSERT_NE(groups, nullptr);
    EXPECT_EQ(genericlist_get_size(groups), 2);

    GenericListGrouping *vowel_group = find_group(groups, "vowel");
    GenericListGrouping *consonant_group = find_group(groups, "consonant");

    ASSERT_NE(vowel_group, nullptr);
    ASSERT_NE(consonant_group, nullptr);
    EXPECT_EQ(read_string_list(vowel_group->elements), (std::vector<std::string>{"apple", "orange", "umbrella"}));
    EXPECT_EQ(read_string_list(consonant_group->elements), (std::vector<std::string>{"banana", "pear"}));

    genericlist_groupby_free(&groups);
    genericlist_free(&words);
}

TEST_F(HofDemo, OrderWordsLexicographicallyUsesStrcmp)
{
    GenericList words = create_string_list({"pear", "apple", "banana", "apricot"});
    ASSERT_NE(words, nullptr);

    GenericList ordered = hofdemo_order_words_lexicographically(words);

    ASSERT_NE(ordered, nullptr);
    EXPECT_EQ(read_string_list(ordered), (std::vector<std::string>{"apple", "apricot", "banana", "pear"}));

    genericlist_free(&ordered);
    genericlist_free(&words);
}

TEST_F(HofDemo, UniqueWordsAggregatesWithoutDuplicates)
{
    GenericList words = create_string_list({"apple", "banana", "apple", "pear", "banana", "pear", "kiwi"});
    ASSERT_NE(words, nullptr);

    GenericList unique_words = hofdemo_unique_words(words);

    ASSERT_NE(unique_words, nullptr);
    EXPECT_EQ(read_string_list(unique_words), (std::vector<std::string>{"apple", "banana", "pear", "kiwi"}));

    genericlist_free(&unique_words);
    genericlist_free(&words);
}

} // namespace