/*----------------------------------------------------------
 *				HTBLA-Leonding
 * ---------------------------------------------------------
 * Title:			Count Character Occurrence
 * Author:			S. Schraml
 * ----------------------------------------------------------
 * Description:
 * Recursive and iterative implementation of an algorithm that 
 * counts the occurrences of a specific character within a string.
 * ----------------------------------------------------------
 */

#include "count_char.h"

#include <stdio.h>
#include <string.h>

/*
Instructions:

Define a function count_char(s, c) that takes a string `s` and the character 'c' to count as input.
Increase the counter by one, if the character matches the character at
the current position within the string and add the count of that character in the remaining string
until the end of the string ('\0').

*/
#include <stdio.h>

// the recursive implementation
int count_char_recursive(const char* str, char c)
{
    char* curr = str;

    if (*curr == '\0')
        return 0;

    return (*curr == c) ? 1 + count_char_recursive(++curr, c) : count_char_recursive(++curr, c);
}

// the iterative implementation
int count_char_iterative(const char* str, char c)
{
    int count = 0;

    while (*str != '\0')
    {
        if (*str == c)
            count++;
        
        str++;
    }

    return count;
}
