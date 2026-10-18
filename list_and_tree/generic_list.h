#ifndef _GENERIC_LIST_H
#define _GENERIC_LIST_H

#include <stddef.h>
#include <stdbool.h>

typedef struct GenericListData* GenericList;

/**
 * Creates a 'new' list instance.
 * Any list obtained via this function MUST be released using
 * function `list_free()`.
 * 
 * Note: This function does not make any assumptions 
 * about how list components, esp. nodes, are allocated.
 * 
 * @return The list instance or 0, if no list could by instantiated.
 */
GenericList genericlist_new();

/**
 * Releases a list that was obtained earlier via function `list_new`.
 * Released lists MUST NOT be used anymore.
 * 
 * Note: The implementation of this function does not make any assumptions
 * about the allocation method of list elements, but MUST match the implementation
 * of function `list_new` as its inverse function.
 * 
 * @param p_list The pointer to the list to release. The value of the pointer
 * is set to 0, if the list was successfully released, otherwise it is left untouched.
 */
void genericlist_free(GenericList* p_list);


/**
 * Checks if a list is valid.
 * 
 * @param list The list to check.
 * @return true if the list is valid, false otherwise.
 */
bool genericlist_is_valid(GenericList list);

/**
 * Checks if a list is empty.
 * 
 * @param list The list to check.
 * @return true if the list is empty, false otherwise.
 */
bool genericlist_is_empty(GenericList list);

/**
 * Gets the nr of elements currently in list.
 * 
 * @param list The list to get the size of.
 * @return The nr of elements currently in list.
 */
int genericlist_get_size(GenericList list);

/** Insert a copy of the given data as payload.
 *  It creates a copy and owns it. */
void genericlist_insert(GenericList list, void* data, unsigned int size);

/**
 * Returns pointer to internal copy of element at index.
 * 
 * @param list The list to get the element from.
 * @param index The index of the element to get.
 * @return Pointer to the internal copy of the element at the specified index.
 */
void* genericlist_get_at(GenericList list, unsigned int index);

/**
 * Removes the element at the specified index.
 * The payload of the removed element is NOT freed by the list!
 * It is the responsibility of the caller to free the payload if necessary.
 * If the index is out of bounds, the list remains unchanged.
 * @param list The list from which to remove the element.
 * @param index The index of the element to remove.
 */
void* genericlist_remove_at(GenericList list, unsigned int index);

/**
 * Clears the list, removing all elements.
 * The payloads of the removed elements are also freed by the list!
 * @param list The list to clear.
 */
void genericlist_clear(GenericList list);

/* Iterators */
typedef void* GenericListIterator;

/** Returns an iterator pointing to the first element of the list.
 *  The iterator is valid as long as the list is not modified.
 *  @param list The list for which to create an iterator.
 *  @return An iterator pointing to the first element of the list.
 * NULL, if the list is empty.
 */
GenericListIterator genericlist_iterator_first(GenericList list);
/** Moves the iterator to the next element.
 *  @param it The iterator to move.
 *  @return The updated iterator.
 * NULL, if the iterator reaches the end of the list.
 */
GenericListIterator genericlist_iterator_move_next(GenericListIterator it);

/** Returns a pointer to the value of the element pointed to by the iterator.
 *  @param it The iterator.
 *  @return A pointer to the value of the element pointed to by the iterator.
 * NULL, if the iterator is invalid.
*/
void* genericlist_iterator_value(GenericListIterator it);

/** Returns the size in bytes of the element. */
size_t genericlist_iterator_sizeof(GenericListIterator it);

#endif