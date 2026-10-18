#ifndef _INT_STRING_DICTIONARY_H
#define _INT_STRING_DICTIONARY_H

typedef struct IntStringPair
{
    int key;
    char *value;
} IntStringPair;

typedef struct IntStringDictionaryRootNode* IntStringDictionary;

IntStringDictionary intstrdict_new();

void intstrdict_free(IntStringDictionary dictionary);

void intstrdict_insert(IntStringDictionary dictionary, int key, const char *value);

void intstrdict_remove(IntStringDictionary dictionary, int key);

int intstrdict_contains(IntStringDictionary dictionary, int key);

char *intstrdict_lookup(IntStringDictionary dictionary, int key);

#endif