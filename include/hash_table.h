#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <string.h>

#include "token.h"

typedef struct Ht_item {
    Token* token;
    struct Ht_item* next;
} Ht_item;

typedef struct Hastable{
    Ht_item** items;
    size_t size;
} HashTable;

HashTable* ht_create(size_t size);
void ht_free(HashTable* ht);
int ht_insert(HashTable* ht, Token* token);
Token* ht_search(HashTable* ht, const char* key);
int ht_delete(HashTable* ht, const char* key);

#endif // HASH_TABLE_H
