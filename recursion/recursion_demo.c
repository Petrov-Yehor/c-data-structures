/*----------------------------------------------------------
 *				HTBLA-Leonding
 * ---------------------------------------------------------
 * Title:			Running recursive and iterative algorithms
 * Author:			S. Schraml
 * ----------------------------------------------------------
 */

#include "sum_of_digits.h"
#include "count_char.h"
#include "reverse_string.h"

#include <stdio.h>
#include <string.h>

int main() {
    printf("\n===========================================\n");
    printf("Executing recursive and iterative algorithms:\n");
    int num = 1234;
    printf("Sum of digits of %d is: %d (recursive) / %d (iterative)\n", num, 
        sum_of_digits_recursive(num), sum_of_digits_iterative(num));
    
    char str[] = "banana";
    char target = 'a';
    printf("The character '%c' appears %d (recursive) / %d (iterative) times in \"%s\".\n", target, 
        count_char_recursive(str, target), count_char_iterative(str, target), str);
    
    char str1[] = "hello";
    char str2[] = "world";
    reverse_string_recursive(str1, 0, strlen(str1) - 1);
    reverse_string_iterative(str2, 0, strlen(str2) - 1);
    printf("Reversed string: %s (recursive) / %s (iterative)\n", str1, str2);

    printf("===========================================\n\n");
    return 0;
}