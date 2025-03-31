#include <stdio.h>
#include "hash_table.h"

static unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

HashTable* ht_create(size_t size) {
    HashTable* ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->size = size;
    ht->items = calloc(ht->size, sizeof(Ht_item*));
    if (!ht->items) {
        free(ht);
        return NULL;
    }
    return ht;
}

void ht_free(HashTable* ht) {
    for (size_t i = 0; i < ht->size; i++) {
        while (ht->items[i]) {
            ht_delete(ht, ht->items[i]->token->key);
        }
    }
    free(ht->items);
    free(ht);
}

int ht_insert(HashTable* ht, Token* token) {
    unsigned long index = hash_function(token->key) % ht->size;
    Ht_item* new_item = malloc(sizeof(Ht_item));
    if (!new_item) return -1;
    new_item->token = token;
    new_item->next = ht->items[index];
    ht->items[index] = new_item;

    return 0;
}

Token* ht_search(HashTable* ht, const char* key) {
    unsigned long index = hash_function(key) % ht->size;
    Ht_item* item = ht->items[index];
    while (item) {
        if (strcmp(item->token->key, key) == 0) {
            return item->token;
            
        }
        item = item->next;
    }
    return NULL;
}

int ht_delete(HashTable* ht, const char* key) {
    unsigned long index = hash_function(key) % ht->size;
    Ht_item* item = ht->items[index];
    Ht_item* prev = NULL;
    while (item) {
        if (strcmp(item->token->key, key) == 0) {
            if (prev)
                prev->next = item->next;
            else
                ht->items[index] = item->next;
            free(item->token->key);
            free(item->token);
            free(item);
            return 0;
        }
        prev = item;
        item = item->next;
    }
    return -1;
}
