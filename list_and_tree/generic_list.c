#include "generic_list.h"
#include "allocator.h"
#include <memory.h>
#include <string.h>

typedef struct GenericListNode
{
    void *data;
    unsigned int size;
    struct GenericListNode *next;
} GenericListNode;

typedef struct GenericListData
{
    GenericListNode *head;
    GenericListNode *tail;
    int size;
} GenericListData;

static GenericListNode *genericlist_create_node(void *data, unsigned int size)
{
    if (data == 0 || size == 0)
    {
        return 0;
    }

    GenericListNode *node = (GenericListNode *)alloc_mem(sizeof(GenericListNode));
    if (node == 0)
    {
        return 0;
    }

    node->data = alloc_mem(size);
    if (node->data == 0)
    {
        free_mem(node);
        return 0;
    }

    memcpy(node->data, data, size);
    node->size = size;
    node->next = 0;
    return node;
}

GenericList genericlist_new()
{
    GenericListData *list = (GenericListData *)alloc_mem(sizeof(GenericListData));
    if (list == 0)
    {
        return 0;
    }

    list->head = 0;
    list->tail = 0;
    list->size = 0;
    return list;
}

void genericlist_free(GenericList *p_list)
{
    if (p_list == 0 || *p_list == 0)
    {
        return;
    }

    genericlist_clear(*p_list);
    free_mem(*p_list);
    *p_list = 0;
}

bool genericlist_is_valid(GenericList list)
{
    return list != 0;
}

bool genericlist_is_empty(GenericList list)
{
    return list == 0 || list->size == 0;
}

int genericlist_get_size(GenericList list)
{
    if (list == 0)
    {
        return 0;
    }
    return list->size;
}

void genericlist_insert(GenericList list, void *data, unsigned int size)
{
    if (list == 0)
    {
        return;
    }

    GenericListNode *node = genericlist_create_node(data, size);
    if (node == 0)
    {
        return;
    }

    if (list->head == 0)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
}

void *genericlist_get_at(GenericList list, unsigned int index)
{
    if (list == 0 || index >= (unsigned int)list->size)
    {
        return 0;
    }

    GenericListNode *current = list->head;
    unsigned int i = 0;
    while (current != 0 && i < index)
    {
        current = current->next;
        i++;
    }

    if (current == 0)
    {
        return 0;
    }
    return current->data;
}

void *genericlist_remove_at(GenericList list, unsigned int index)
{
    if (list == 0 || index >= (unsigned int)list->size)
    {
        return 0;
    }

    GenericListNode *prev = 0;
    GenericListNode *current = list->head;
    unsigned int i = 0;
    while (current != 0 && i < index)
    {
        prev = current;
        current = current->next;
        i++;
    }

    if (current == 0)
    {
        return 0;
    }

    if (prev == 0)
    {
        list->head = current->next;
    }
    else
    {
        prev->next = current->next;
    }

    if (list->tail == current)
    {
        list->tail = prev;
    }

    void *payload = current->data;
    free_mem(current);
    list->size--;
    return payload;
}

void genericlist_clear(GenericList list)
{
    if (list == 0)
    {
        return;
    }

    GenericListNode *current = list->head;
    while (current != 0)
    {
        GenericListNode *next = current->next;
        free_mem(current->data);
        free_mem(current);
        current = next;
    }

    list->head = 0;
    list->tail = 0;
    list->size = 0;
}

GenericListIterator genericlist_iterator_first(GenericList list)
{
    if (list == 0)
    {
        return 0;
    }
    return (GenericListIterator)list->head;
}

GenericListIterator genericlist_iterator_move_next(GenericListIterator it)
{
    if (it == 0)
    {
        return 0;
    }

    GenericListNode *node = (GenericListNode *)it;
    return (GenericListIterator)node->next;
}

void *genericlist_iterator_value(GenericListIterator it)
{
    if (it == 0)
    {
        return 0;
    }

    GenericListNode *node = (GenericListNode *)it;
    return node->data;
}

size_t genericlist_iterator_sizeof(GenericListIterator it)
{
    if (it == 0)
    {
        return 0;
    }

    GenericListNode *node = (GenericListNode *)it;
    return node->size;
}