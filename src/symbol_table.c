#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "hash_table.h"
#include "constants.h"
#include "error_handler.h"

#define HASHTABLE_SIZE 101

typedef struct SymbolTable {
    HashTable* ht;
} SymbolTable;

SymbolTable* st;

int st_init(Token** keywords, int size) {
    st = malloc(sizeof(SymbolTable));
    if (!st) {
        eh_printerr("Malloc failed", FATAL_ERROR, 0);
        exit(EXIT_FAILURE);
    }
    st->ht = ht_create(HASHTABLE_SIZE);
    if (!st->ht) {
        free(st);
        return -1;
    }

    for (int i = 0; i < size; i++) {
        ht_insert(st->ht, keywords[i]);
    }

    return 0;
}

Token* st_search(const char* key, int id, Value value) {
    Token* token = NULL;
    if (ht_search(st->ht, key, &token) == -1) {
        token = malloc(sizeof(Token));
        if (!token) {
            eh_printerr("Malloc failed", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        token->key = strdup(key);
        token->id = id;
        token->value = value;
        ht_insert(st->ht, token);
    }
    return token;
}

void st_free(void) {
    if (!st) return;
    ht_free(st->ht);
    free(st);
}

void st_print(void) {
    if (!st) {
        printf("\n\nSymbolTable is NULL.\n");
        return;
    }

    printf("\n\n\n========= SYMBOL TABLE STATE =========\n\n");

    for (size_t i = 0; i < st->ht->size; i++) {
        Ht_item* item = st->ht->items[i];

        while (item) {
            Token* token = item->token;

            if(token->id != FUNCTION){            
                printf("< %s, %d, %.2lf >\n", token->key, token->id, token->value.var);
            }else{
                printf("< %s, %d, %p >\n", token->key, token->id, token->value.fnctptr);
            }

            item = item->next;
        }
    }
    
    printf("\n====================================\n\n\n");
}