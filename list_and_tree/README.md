# Higher Order Functions and a Binary Tree

## Higher Order Functions on lists

Given the implementation of a generic list in `generic_list_functions` and a set of higher order functions in `generic_list_functions`, implement the following tasks in a module `higher_order_function_demo.c`. Google Test Testcases are defined in `test_higher_order_function_demo.cpp` in test suite `HofDemo`. The module and the functions bodies are already there.

- list of integers, calculate sum of odd numbers
- list of integers, extract every 5th element
- list of strings, group into words starting with a consonant or a vowel (English language).
- list of strings, order lexicographically using strcmp
- list of strings, aggregate into list without duplicates

## Implement binary full deletion

Using post-order traversal, tidy up and free the binary tree in `binary_tree/intbintree_free`.

## Implement tree traversal functions

Given the binary tree, implement the traversal functions for `intbintree_apply` and `intbintree_apply_with_context` in `binary_tree_traversal`. Some helper functions are already there.

## Extend the binary int tree to be a dictionary of int to char*

Copy the implementation of `binary_tree` to `int_string_dictionary` and implement a dictionary of int to string. The struct and bodies are already there.

- Define a struct IntStringPair holding an int and a char*.
- Use this as the payload of the tree.
- On insertion, the given string is copied, such that the tree holds its own copy.
- If the key is already in the tree, the string is replaced.
- Traversal of the tree uses the integer.
- On lookup, return the char*.
- Don't forget to clean up everything on deletion!
