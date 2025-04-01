#ifndef FUNCTIONS_H
#define FUNCTIONS_H


extern FILE* current_fp;

void init_fncts();

void fncts_free();

void IMPORT(const char* libname);

void *find_symbol(const char *sym);

void LOAD(const char* unused);

void QUIT(const char* unused);

void HELP(const char* unused);

void WORKSPACE(const char* unused);

void CLEAR(const char* unused);

void CLEAN(const char* unused);

void ECHO(const char* unused);

#endif // FUNCTIONS_H
