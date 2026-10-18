# C Data Structures

My C coursework from HTL Leonding (2nd year, procedural programming): a doubly linked list, a binary search tree, generic higher-order list functions, a CSV parser and recursion exercises. Every module is covered by GoogleTest unit tests.

The exercises came with a starter project from my teacher: header files, a memory allocator for tracking leaks, makefiles and the unit tests. My job was to implement the missing functions until the tests passed. The tables below show what I wrote and what was provided.

## Projects

| Folder | Topic |
|---|---|
| [`dlinked_list`](dlinked_list) | Doubly linked list of integers |
| [`list_and_tree`](list_and_tree) | Generic list with higher-order functions, binary search tree |
| [`csvParser`](csvParser) | Reading, validating and sorting records from a CSV file |
| [`recursion`](recursion) | Recursive vs. iterative algorithms, merge sort |

### dlinked_list: doubly linked list

Each node points to both its previous and its next node, and the list keeps pointers to its head and tail.

**I implemented:** `list_contains`, `list_get_at`, `list_insert_at` and `list_remove_at`. Inserting and removing in the middle of the list means updating up to four pointers in the right order, plus the special cases for the head, the tail and an empty list.

### list_and_tree: higher-order functions and a binary tree

A generic list stores `void*` payloads, so the same code works for integers, strings or structs. Behaviour is passed in as **function pointers**, which is how C does what other languages do with lambdas.

**I implemented:**

- **Higher-order list functions** (`generic_list_functions.c`), similar to LINQ in C# or streams in Java:
  - `where`: filter by a predicate
  - `select`: map with a transformer
  - `orderby`: sort with a comparator
  - `groupby`: group by a key selector
  - `aggregate`: fold into an accumulator
- **Using them** (`higher_order_function_demo.c`): the sum of odd numbers, every 5th element, grouping words by vowel/consonant, sorting strings with `strcmp`, and removing duplicates.
- **Tree traversal** (`binary_tree_traversal.c`): in-order, pre-order and post-order, done recursively and with a callback applied to each node.
- **Freeing the tree** (`intbintree_free`): a post-order traversal, so children are freed before their parent.

### csvParser: CSV parser

This reads a CSV file of persons, skips invalid lines, prints a formatted table and sorts the records.

**I implemented** (`person.c`):

- building `PersonRecord`s from the file lines;
- printing the valid records, with Unix timestamps converted to `DD.MM.YYYY`;
- comparing records by a selected field;
- a selection sort that uses that comparison.

### recursion: recursion exercises

**I implemented** recursive and iterative versions of `count_char`, `reverse_string` and `sum_of_digits`, plus a recursive **merge sort**.



## What I learned

- Manual memory management: every `malloc` needs exactly one `free`, and the allocator in the tests catches leaks immediately.
- Pointer manipulation in linked structures, and why the order of pointer updates matters.
- Function pointers and `void*` as the C way to write generic code.
- Recursion, and when an iterative solution is simpler.
- Reading the tests first and implementing against them.
