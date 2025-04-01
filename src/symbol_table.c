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

Token* st_search(const char* key) {
    return ht_search(st->ht, key);
}

int st_insert(Token* token){
    return ht_insert(st->ht, token);
}

void st_free(void) {
    if (!st) return;
    ht_free(st->ht);
    free(st);
}

void st_clean_items(int full){
    for (size_t i = 0; i < st->ht->size; i++) {
        Ht_item* item = st->ht->items[i];
        Ht_item* next = NULL;

        while (item) {
            Token* token = item->token;
            next = item->next; 

            if(token->id == IDENTIFIER || (full && token->id == CONSTANT)){            
                if ((ht_delete(st->ht, token->key)) == -1){
                    eh_printerr("hash item could not be deleted", FATAL_ERROR, 0);
                    exit(EXIT_FAILURE);
                };
            }

            item = next;
        }
    }
}

void st_print_items(int type) {
    for (size_t i = 0; i < st->ht->size; i++) {
        Ht_item* item = st->ht->items[i];
        while (item) {
            Token* token = item->token;
            if (token->id == type) {
                printf("| %-9s --> %9.4lf |\n",
                       token->key, token->value.var);
            }
            item = item->next;
        }
    }
}

void st_clean_consts(void){
    for (size_t i = 0; i < st->ht->size; i++) {
        Ht_item* item = st->ht->items[i];
        Ht_item* next = NULL;

        while (item) {
            Token* token = item->token;

            if ((ht_delete(st->ht, token->key)) == -1){
                eh_printerr("hash item could not be deleted", FATAL_ERROR, 0);
                exit(EXIT_FAILURE);
            };

            item = next;
        }
    }
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

            if(token->id == IDENTIFIER || token->id ==  CONSTANT ){            
                printf("< %s, %d, %.2lf >\n", token->key, token->id, token->value.var);
            }else{
                printf("< %s, %d, %p >\n", token->key, token->id, token->value.fnctptr);
            }

            item = item->next;
        }
    }
    
    printf("\n====================================\n\n\n");
}