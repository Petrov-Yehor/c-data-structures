#include "int_string_dictionary_internal.h"

#include <string.h>

#include "allocator.h"


static char *copy_string(const char *value)
{
    char *copy;
    size_t length;

    if (value == NULL)
    {
        return NULL;
    }

    length = strlen(value) + 1;
    copy = alloc_mem(length);
    if (copy != NULL)
    {
        memcpy(copy, value, length);
    }

    return copy;
}

static IntStringDictionaryNode *create_node(int key, const char *value)
{
    IntStringDictionaryNode *node;
    char *copied_value;

    copied_value = copy_string(value);
    if (copied_value == NULL)
    {
        return NULL;
    }

    node = alloc_mem(sizeof(IntStringDictionaryNode));
    if (node == NULL)
    {
        free_mem(copied_value);
        return NULL;
    }

    node->payload.key = key;
    node->payload.value = copied_value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void free_payload(IntStringPair *payload)
{
    if (payload->value != NULL)
    {
        free_mem(payload->value);
        payload->value = NULL;
    }
}

static void free_subtree(IntStringDictionaryNode *node)
{
   
}

static void find_node_with_parent(IntStringDictionary dictionary,
                                  int key,
                                  IntStringDictionaryNode **parent,
                                  IntStringDictionaryNode **current)
{
   
}

static void remove_leaf_node(IntStringDictionary dictionary,
                             IntStringDictionaryNode *parent,
                             IntStringDictionaryNode *current)
{
   
}

static void remove_single_child_node(IntStringDictionary dictionary,
                                     IntStringDictionaryNode *parent,
                                     IntStringDictionaryNode *current)
{
   
}

static void remove_two_children_node(IntStringDictionaryNode *current)
{
   
}

IntStringDictionary intstrdict_new()
{
    IntStringDictionary dictionary = alloc_mem(sizeof(struct IntStringDictionaryRootNode));

    if (dictionary != NULL)
    {
        dictionary->root = NULL;
        dictionary->size = 0;
    }

    return dictionary;
}

void intstrdict_free(IntStringDictionary dictionary)
{
   
}

void intstrdict_insert(IntStringDictionary dictionary, int key, const char *value)
{
   
}

void intstrdict_remove(IntStringDictionary dictionary, int key)
{
   
}

int intstrdict_contains(IntStringDictionary dictionary, int key)
{
    return intstrdict_lookup(dictionary, key) != NULL;
}

char *intstrdict_lookup(IntStringDictionary dictionary, int key)
{
   

    return NULL;
}