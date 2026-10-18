/*----------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class>
 * ---------------------------------------------------------
 * Title:			Reverse String
 * Author:			S. Schraml
 * ----------------------------------------------------------
 * Description:
 * Recursive and iterative implementation of an algorithm that 
 * reverses a given string.
 * ----------------------------------------------------------
 */

#include "reverse_string.h"

#include <stdio.h>
#include <string.h>

/*
Instructions:

Define a function reverse_string(s) that takes a string `s` as input.
Take the last character of `s` and concatenate it with the result of reversing the rest of the string until the string has less than two characters.

Note that strings that are stored on heap or stack are provided - they can be directly manipulated.

*/
#include <stdio.h>

// the recursive implementation
void reverse_string_recursive(char* str, int start, int end)
{
    if (start >= end)
        return;
    
    swap_chars(str, start, end);

    return reverse_string_iterative(str, ++start, --end);
}

// the iterative implementation
void reverse_string_iterative(char* str, int start, int end)
{
    char* temp = str;

    for (int i = 0; i < end; i++)
    {
        swap_chars(str, start, end);
        start++;
        end--;
    }
}

// not mem safe lol
void swap_chars(char* str, int a, int b)
{
    char tmp = str[a];
    str[a] = str[b];
    str[b] = tmp;
}