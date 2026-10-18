#include <stdio.h>
#include "generic_list_functions.h"
#include "allocator.h"

GenericList genericlist_where(GenericList l, predicate p)
{
    GenericList result = genericlist_new();

    GenericListIterator it = genericlist_iterator_first(l);
    void *data = NULL;
    while ((data = genericlist_iterator_value(it)) != NULL)
    {
        if (p(data) == 1)
        {
            size_t elemSize = genericlist_iterator_sizeof(it);
            genericlist_insert(result, data, elemSize);
        }
        it = genericlist_iterator_move_next(it);
    }
    return result;
}

GenericList genericlist_select(GenericList l, transformer t)
{
    GenericList result = genericlist_new();

    GenericListIterator it = genericlist_iterator_first(l);
    void *data = NULL;
    while ((data = genericlist_iterator_value(it)) != NULL)
    {
        size_t newElemSize = 0;
        void *newData = t(data, &newElemSize);

        genericlist_insert(result, newData, newElemSize);
        it = genericlist_iterator_move_next(it);
    }
    return result;
}

void genericlist_aggregate(GenericList l, aggregator a, void *accumulator)
{
    GenericListIterator it = genericlist_iterator_first(l);
    void *data = NULL;
    while ((data = genericlist_iterator_value(it)) != NULL)
    {
        a(data, accumulator);
        it = genericlist_iterator_move_next(it);
    }
}

int genericlist_foreach(GenericList l, void (*action)(void *data))
{
    int count = 0;
    GenericListIterator it = genericlist_iterator_first(l);
    void *data = NULL;
    while ((data = genericlist_iterator_value(it)) != NULL)
    {
        action(data);
        count++;
        it = genericlist_iterator_move_next(it);
    }
    return count;
}

GenericList genericlist_orderby(GenericList l, comparator c)
{
    // for simplicity, perform a selection sort on the list.
    // Idea to make it easy to implement:
    // 1 extract iterators of given list into array
    // 2 sort using the array of iterators and the comparator c
    // 3 create new result list by inserting values from the array of iterators
    int nrOfElements = genericlist_get_size(l);
    GenericListIterator *nodes = alloc_mem(nrOfElements * sizeof(GenericListIterator));
    int index = 0;
    GenericListIterator it = genericlist_iterator_first(l);
    while (it != NULL)
    {
        nodes[index++] = it;
        it = genericlist_iterator_move_next(it);
    }

    //selection sort for simplicity; could be any other sort 
    for (int i = 0; i < nrOfElements; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < nrOfElements; j++)
        {
            GenericListIterator minNode = nodes[minIndex];
            GenericListIterator otherNode = nodes[j];

            if (c(genericlist_iterator_value(otherNode), genericlist_iterator_value(minNode)) < 0)
            {
                minIndex = j;
            }
        }
        if(minIndex != i)
        {
            // swap nodes[i] and nodes[minIndex]
            GenericListIterator tmp = nodes[i];
            nodes[i] = nodes[minIndex];
            nodes[minIndex] = tmp;
        }
    }

    //create result list from sorted array of iterators
    GenericList result = genericlist_new();
    for (int i = 0; i < nrOfElements; i++)
    {
        genericlist_insert(result, genericlist_iterator_value(nodes[i]), genericlist_iterator_sizeof(nodes[i]));
    }

    free_mem(nodes);

    return result;
}

GenericListIterator genericlist_grouping_find_group(GenericList groups, void *key, comparator c)
{
    GenericListIterator groupIt = genericlist_iterator_first(groups);
    GenericListGrouping *grouping = NULL;
    while ((grouping = (GenericListGrouping *)genericlist_iterator_value(groupIt)) != NULL)
    {
        if (c(grouping->key, key) == 0)
        {
            return groupIt; // Found the group
        }
        groupIt = genericlist_iterator_move_next(groupIt);
    }
    return NULL; // Group not found
}

GenericList genericlist_groupby(GenericList l, keyselector k, comparator c)
{
    GenericList groups = genericlist_new();

    GenericListIterator dataIter = genericlist_iterator_first(l);
    void *data = NULL;
    while ((data = genericlist_iterator_value(dataIter)) != NULL)
    {
        void *key = k(data);

        GenericListIterator groupIt = genericlist_grouping_find_group(groups, key, c);
        GenericListGrouping *grouping = NULL;
        GenericListGrouping newGrouping;
        GenericList groupingList = NULL;
        if (groupIt == NULL)
        {
            newGrouping.key = key;
            groupingList = newGrouping.elements = genericlist_new();
            genericlist_insert(groups, &newGrouping, sizeof(GenericListGrouping));
        }
        else
        {
            grouping = (GenericListGrouping *)genericlist_iterator_value(groupIt);
            groupingList = grouping->elements;
        }
            
            // Add the element to the group's list of elements
            size_t elemSize = genericlist_iterator_sizeof(dataIter);
            genericlist_insert(groupingList, data, elemSize);

        dataIter = genericlist_iterator_move_next(dataIter);
    }
    return groups;
}

static void genericlist_grouping_free_action(void *data)
{
    GenericListGrouping *grouping = (GenericListGrouping *)data;
    genericlist_free(&grouping->elements);
}

void genericlist_groupby_free(GenericList *p_groups)
{
    if (p_groups == NULL || *p_groups == NULL)
    {
        return;
    }

    genericlist_foreach(*p_groups, genericlist_grouping_free_action);
    genericlist_free(p_groups);
}