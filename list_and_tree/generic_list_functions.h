#ifndef _GENERIC_LIST_FUNCTIONS
#define _GENERIC_LIST_FUNCTIONS

#include "generic_list.h"

/*Functions and types for the generic list operations */

/** Predicate implements a condition. Returns 1 if true, 0 if false */
typedef int (*predicate)(void *data);

/** Transformer creates a new element from given data to be inserted into the generic list. 
 * It writes the size in bytes of the new element to newElemeSize.
*/
typedef void* (*transformer)(void *data, size_t *newElemSize);

/** Action takes an element and performs some operation on it, without returning a value. */
typedef void (*action)(void *data);

/** COMPARATOR returns -1 if data1 < data2, 0 if data1 == data2, 1 if data1 > data2.
 * It is assumed that the comparing function can determine the size and type of the elements to be compared.
*/
typedef int (*comparator)(void *data1, void *data2);

/** Aggregator takes an element and the accumulator. It then calculates a new accumulator value. */
typedef void (*aggregator)(void *data, void *accumulator);

/** KeySelector takes an element and returns a key for grouping.
 * Equality between keys is tested with the comparator c */
typedef void* (*keyselector)(void *data);

typedef struct {
    void *key;
    GenericList elements;
} GenericListGrouping;

/* Function prototypes */

/** SELECT returns a new list from l with all elements converted by the transformer t. */
GenericList genericlist_select(GenericList l, transformer t);

/** WHERE returns a new list from l with only elements where predicate is 1 */
GenericList genericlist_where(GenericList l, predicate p);

/** FOREACH applies the action a to each element of the list l. It returns the number of elements processed. */
int genericlist_foreach(GenericList l, void (*action)(void *data));

/** OrderBy returns a new list from l with elements sorted by the comparator c. List must be freed by caller. */
GenericList genericlist_orderby(GenericList l, comparator c);

/** Aggregate returns the result of applying the aggregator a to all elements of l, starting with the value in accumulator.
 */
void genericlist_aggregate(GenericList l, aggregator a, void *accumulator);

/** GroupBy returns a new list of GenericListGroupings, where each group is a list of elements with the same key.
 * The key for each group is determined by the key selector k, and equality between keys is determined by the comparator c. */
GenericList genericlist_groupby(GenericList l, keyselector k, comparator c);

/** Frees a list produced by GroupBy including all nested group element lists. */
void genericlist_groupby_free(GenericList *p_groups);


#endif