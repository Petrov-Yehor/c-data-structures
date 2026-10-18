#ifndef _INT_STRING_DICTIONARY_INTERNAL_H
#define _INT_STRING_DICTIONARY_INTERNAL_H

#include <stddef.h>

#include "int_string_dictionary.h"

typedef struct IntStringDictionaryNode
{
    IntStringPair payload;
    struct IntStringDictionaryNode *left;
    struct IntStringDictionaryNode *right;
} IntStringDictionaryNode;

struct IntStringDictionaryRootNode
{
    IntStringDictionaryNode *root;
    size_t size;
};

#endif