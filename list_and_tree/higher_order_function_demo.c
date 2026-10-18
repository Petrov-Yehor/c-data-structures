#include "higher_order_function_demo.h"

#include <ctype.h>
#include <string.h>

#include "generic_list_functions.h"


//TODO 1 Implement the function to test for an odd number. Returns 1 if the number is odd, 0 otherwise.
static int is_odd(void *data)
{
    int num = *(int*)data;

    return num % 2 == 1 ? 1 : 0;
}

//TODO 1 Implement the function to sum integers. The data parameter is a pointer to an integer to add to the accumulator, which is a pointer to the running total.
static void sum_ints(void *data, void *accumulator)
{
    int num = *(int*)data;
    *(int*)accumulator += num;
}

//TODO 2 Implement the function to collect every fifth element. The data parameter is a pointer to the current element, and the accumulator is a pointer to a GenericList to which every fifth element should be added.
static int every_fifth_counter = 0;
static void collect_every_fifth(void *data, void *accumulator)
{
    GenericList result = (GenericList)accumulator;

    if (every_fifth_counter % 5 == 4)
    {
        genericlist_insert(result, data, sizeof(int));
    }

    every_fifth_counter++;
}

//fully implemented helper functions for the group by demo
static int starts_with_vowel_char(char c)
{
    char lower = (char)tolower((unsigned char)c);
    return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
}

//fully implemented helper function for the group by demo. Returns a pointer to a static string "vowel" if the input word starts with a vowel, and "consonant" otherwise.
static void *word_group_key(void *data)
{
    char *word = (char *)data;
    static char vowel_key[] = "vowel";
    static char consonant_key[] = "consonant";

    if (word == 0 || word[0] == '\0')
    {
        return consonant_key;
    }

    return starts_with_vowel_char(word[0]) ? vowel_key : consonant_key;
}

//fully implemented helper function for the unique words demo. Returns 1 if the candidate string is found in the list of words, 0 otherwise.
static int compare_strings(void *left, void *right)
{
    return strcmp((char *)left, (char *)right);
}

//TODO 5 Implement the function to check if a list contains a string. The words parameter is a GenericList of strings, and candidate is the string to search for. Returns 1 if the candidate is found in the list, 0 otherwise.
static int list_contains_string(GenericList words, char *candidate)
{
    GenericListIterator it = genericlist_iterator_first(words);

    while (it != NULL)
    {
        char *word = genericlist_iterator_value(it);

        if (strcmp(word, candidate) == 0)
            return 1;

        it = genericlist_iterator_move_next(it);
    }

    return 0;
}

//TODO 5 Implement the function to aggregate unique words. The data parameter is a pointer to a string, and the accumulator is a pointer to a GenericList of strings. If the string pointed to by data is not already in the list, it should be added.
static void aggregate_unique_word(void *data, void *accumulator)
{
    GenericList unique_words = (GenericList)accumulator;
    char *word = (char *)data;

    if (list_contains_string(unique_words, word) != 1)
        genericlist_insert(unique_words, word, strlen(word) + 1);
}

//TODO 1 Implement the function to sum odd numbers in a list. The numbers parameter is a GenericList of integers. The function should return the sum of the odd numbers in the list.
int hofdemo_sum_of_odd_numbers(GenericList numbers)
{
    int sum = 0;

    genericlist_aggregate(genericlist_where(numbers, is_odd), sum_ints, &sum);
    
    return sum;
}

//fully implemented, resets the every fifth counter to 0. This is needed to ensure that the hofdemo_extract_every_fifth function returns the expected result when called multiple times in a row.
void hofdemo_reset_every_fifth_counter(void)
{
    every_fifth_counter = 0;
}

//TODO 2 Implement the function to extract every fifth element from a list. The numbers parameter is a GenericList of integers. The function should return a new GenericList containing every fifth element from the input list, starting with the first element (i.e. the 1st, 6th, 11th, etc. elements).
GenericList hofdemo_extract_every_fifth(GenericList numbers)
{
    GenericList result = genericlist_new();

    genericlist_aggregate(numbers, collect_every_fifth, result);

    return result;
}

//TODO 3 Implement the function to group words by their initial character. The words parameter is a GenericList of strings. The function should return a new GenericList of groups, where each group is a GenericList of strings that start with the same character. For example, if the input list contains the words "apple", "apricot", "banana", and "blueberry", the output should be a list containing two groups: one group with "apple" and "apricot", and another group with "banana" and "blueberry".
GenericList hofdemo_group_words_by_initial(GenericList words)
{
    GenericList dummy = genericlist_new();

    dummy = genericlist_groupby(words, word_group_key, compare_strings);

    return dummy;
}

//TODO 4 Implement the function to order words lexicographically. The words parameter is a GenericList of strings. The function should return a new GenericList containing the same strings as the input list, but ordered in lexicographical (alphabetical) order.
GenericList hofdemo_order_words_lexicographically(GenericList words)
{
    GenericList dummy = genericlist_new();

    dummy = genericlist_orderby(words, compare_strings);

    return dummy;
}

//TODO 5 Implement the function to extract unique words from a list. The words parameter is a GenericList of strings. The function should return a new GenericList containing only the unique strings from the input list (i.e. if a string appears multiple times in the input list, it should only appear once in the output list).
GenericList hofdemo_unique_words(GenericList words)
{
    GenericList dummy = genericlist_new();

    genericlist_aggregate(words, aggregate_unique_word, dummy);

    return dummy;
}
