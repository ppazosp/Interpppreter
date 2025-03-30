#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define AUTOINSERT 1
#define NOINSERT 0

#include "token.h"

int st_init(Token** keywords, int size);

Token* st_search(const char* key, int id, Value value);

void st_free(void);

void st_print(void);

#endif // SYMBOL_TABLE_H
