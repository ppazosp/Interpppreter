#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define AUTOINSERT 1
#define NOINSERT 0

#include "token.h"

int st_init(Token** keywords, int size);

Token* st_search(const char* key);

int st_insert(Token* token);

void st_free(void);

void st_print(void);

void st_clean_vars(void);

 void st_clean_consts(void);

#endif // SYMBOL_TABLE_H
