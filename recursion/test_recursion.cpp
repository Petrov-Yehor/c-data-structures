extern "C" {
    #include "count_char.h"
    #include "reverse_string.h"
    #include "sum_of_digits.h"
	#include "mergesort.h"
}

#include <gtest/gtest.h>

TEST(SetupTest, GTestIsHere)
{
    ASSERT_TRUE(1 == 1);
}

///////////////////////////////////////////////////////////////////////////////
//count_char tests
///////////////////////////////////////////////////////////////////////////////
TEST(CountCharTests, test_count_char_rec) {
    char str1[] = "coconut";
	int res = count_char_recursive(str1, 'o');
	ASSERT_EQ(2, res);
	res = count_char_recursive(str1, 'a');
	ASSERT_EQ(0, res);
    char str2[] = "Y";
	res = count_char_recursive(str2, 'y');
	ASSERT_EQ(0, res);
    char str3[] = "";
	res = count_char_recursive(str3, 'z');
	ASSERT_EQ(0, res);
}

TEST(CountCharTests, test_count_char_itr) {
    char str1[] = "coconut";
	int res = count_char_iterative(str1, 'o');
	ASSERT_EQ(2, res);
	res = count_char_iterative(str1, 'a');
	ASSERT_EQ(0, res);
    char str2[] = "Y";
	res = count_char_iterative(str2, 'y');
	ASSERT_EQ(0, res);
    char str3[] = "";
	res = count_char_iterative(str3, 'z');
	ASSERT_EQ(0, res);
}

///////////////////////////////////////////////////////////////////////////////
//reverse_string tests
///////////////////////////////////////////////////////////////////////////////
TEST(ReverseStringTests, test_reverse_string_rec) {
    char str1[] = "testing";
	reverse_string_recursive(str1, 0, strlen(str1) - 1);
	ASSERT_STREQ("gnitset", str1);
    char str2[] = "X";
	reverse_string_recursive(str2, 0, strlen(str2) - 1);
	ASSERT_STREQ("X", str2);
    char str3[] = "";
	reverse_string_recursive(str3, 0, strlen(str3) - 1);
	ASSERT_STREQ("", str3);
}

TEST(ReverseStringTests, test_reverse_string_itr) {
    char str1[] = "testing";
	reverse_string_iterative(str1, 0, strlen(str1) - 1);
	ASSERT_STREQ("gnitset", str1);
    char str2[] = "X";
	reverse_string_iterative(str2, 0, strlen(str2) - 1);
	ASSERT_STREQ("X", str2);
    char str3[] = "";
	reverse_string_iterative(str3, 0, strlen(str3) - 1);
	ASSERT_STREQ("", str3);
}

///////////////////////////////////////////////////////////////////////////////
//sum_of_digits tests
///////////////////////////////////////////////////////////////////////////////
TEST(SumOfDigitsTests, test_sum_of_digits_rec) {
	int res = sum_of_digits_recursive(5);
	ASSERT_EQ(5, res);
	res = sum_of_digits_recursive(42);
	ASSERT_EQ(6, res);
	res = sum_of_digits_recursive(1234);
	ASSERT_EQ(10, res);
}

TEST(SumOfDigitsTests, test_sum_of_digits_itr) {
	int res = sum_of_digits_iterative(3);
	ASSERT_EQ(3, res);
	res = sum_of_digits_iterative(20);
	ASSERT_EQ(2, res);
	res = sum_of_digits_iterative(4321);
	ASSERT_EQ(10, res);
}
 
///////////////////////////////////////////////////////////////////////////////
//Test of Mergesort
///////////////////////////////////////////////////////////////////////////////
#include <algorithm>
 
class MergeSortTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test data
        sorted = {1, 2, 3, 4, 5};
        reverse = {5, 4, 3, 2, 1};
        random = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        expected_random = {1, 1, 2, 3, 3, 4, 5, 5, 6, 9};
    }
 
    std::vector<int> sorted;
    std::vector<int> reverse;
    std::vector<int> random;
    std::vector<int> expected_random;
};
 
// Test mergeSort(int arr[], size_t len)
TEST_F(MergeSortTest, MergeSortLen) {
    std::vector<int> arr = random;
    mergeSort(arr.data(), (size_t)(arr.size()));
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected_random.begin()));
}
 
// Test mergeSort(int arr[], size_t left, size_t right)
TEST_F(MergeSortTest, MergeSortRecursive) {
    std::vector<int> arr = random;
    mergeSortRecursive(arr.data(), 0, (size_t)(arr.size() - 1));
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected_random.begin()));
}
 
// Edge cases
TEST_F(MergeSortTest, SortEmptyArray) {
    std::vector<int> arr = {};
    mergeSort(arr.data(), (size_t)(arr.size()));
    EXPECT_TRUE(arr.empty());
}
 
TEST_F(MergeSortTest, SortSingleElement) {
    std::vector<int> arr = {42};
    mergeSort(arr.data(), (size_t)(arr.size()));
    EXPECT_EQ(arr[0], 42);
}
 
TEST_F(MergeSortTest, SortAlreadySorted) {
    std::vector<int> arr = sorted;
    mergeSort(arr.data(), (size_t)(arr.size()));
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), sorted.begin()));
}
 
TEST_F(MergeSortTest, SortReverseSorted) {
    std::vector<int> arr = reverse;
    mergeSort(arr.data(), (size_t)(arr.size()));
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), sorted.begin()));
}

// Test merge function directly
TEST_F(MergeSortTest, MergeFunction) {
    std::vector<int> arr = {1, 3, 5, 2, 4, 6};
    std::vector<int> expected = {1, 2, 3, 4, 5, 6};
    merge(arr.data(), 0, 2, 5);
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected.begin()));
}
 
// Test merge with overlapping ranges
TEST_F(MergeSortTest, MergeRightArrayLonger) {
	// [1, 3] [2, 4, 5]
	//  0  1   2  3  4
    std::vector<int> arr = {1, 3, 2, 4, 5};
    std::vector<int> expected = {1, 2, 3, 4, 5};
    merge(arr.data(), 0, 1, 4);
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected.begin()));
}

TEST_F(MergeSortTest, MergeLeftArrayLonger) {
	// [1, 2, 3] [4, 5]
	//  0  1   2  3  4
    std::vector<int> arr = {1, 2, 3, 4, 5};
    std::vector<int> expected = {1, 2, 3, 4, 5};
    merge(arr.data(), 0, 2, 4);
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected.begin()));
}

// Test merge with empty halves
TEST_F(MergeSortTest, MergeEmptyHalve) {
    std::vector<int> arr = {1, 3};
    std::vector<int> expected = {1, 3};
    merge(arr.data(), 0, 1, 1);
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected.begin()));
}
 