/*----------------------------------------------------------
 *				HTBLA-Leonding
 * ---------------------------------------------------------
 * Title:			Reverse String
 * Author:			S. Schraml
 * ----------------------------------------------------------
 * Description:
 * Recursive and iterative implementation of an algorithm that 
 * reverses a given string.
 * ----------------------------------------------------------
 */

/* *************************************************************
 *     see corresponding c-file for instructions
 ***************************************************************/

#ifndef ___REVERSE_STRING_H
#define ___REVERSE_STRING_H

#include <stdbool.h>

void reverse_string_recursive(char* str, int start, int end);
void reverse_string_iterative(char* str, int start, int end);

#endif