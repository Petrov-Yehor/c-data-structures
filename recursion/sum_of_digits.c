/*----------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class>
 * ---------------------------------------------------------
 * Title:			Sum of Digits
 * Author:			S. Schraml
 * ----------------------------------------------------------
 * Description:
 * Recursive and iterative implementation of an algorithm that calculates 
 * the sum of the digits of a given positive integer.
 * ----------------------------------------------------------
 */
#include "sum_of_digits.h"

#include <stdio.h>
#include <string.h>

/*
Instructions:

Define a function sum_of_digits(n) that takes a positive integer `n` as input.
The sum of digits can be calculated as n mod 10 + sum of digits of n / 10 as long as n is larger than ten.

*/
#include <stdio.h>

// the recursive implementation
int sum_of_digits_recursive(int n)
{
    if (n < 10)
        return n;
    
    return n % 10 + sum_of_digits_recursive(n / 10);
}

// the iterative implementation
int sum_of_digits_iterative(int n)
{
    if (n < 0)
        n = -n;

    int sum = 0;

    while (n > 0)
    {
        sum += n % 10;
        n /= 10;
    }

    return sum;
}
