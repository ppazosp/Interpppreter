#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "functions.h"
#include "symbol_table.h"
#include "constants.h"
#include "error_handler.h"
#include "stack.h"

#define STACK_SIZE 8

Stack *stack;

typedef struct Library {
    char *name;         
    void *handle;      
    struct Library *next;
} Library;

int echo = TRUE;

void init_functions(){
    stack = stack_create(STACK_SIZE);
}

void IMPORT(const char* libname){
    void *handle;
    handle = dlopen(libname, RTLD_LAZY); 
    if (!handle) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "Could not open lib %s", libname);
        yyerror(buffer);
        return;
    }
}

void LOAD(const char* filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "Could not open file %s", filename);
        yyerror(buffer);
        return;
    }
    printf("Reading from new input...\n");
    stack_push(stack, current_fp);
    current_fp = fp;
}

void QUIT(const char* unused){
    if (current_fp != NULL) {
        fclose(current_fp);
        current_fp = stack_pop(stack);
        printf("Returning to previous input...\n\n\n");
    } else {
        st_print();
        st_free();
        exit(EXIT_SUCCESS);
    }
}

void HELP(const char* unused){
    printf("HELP");
}

void WORKSPACE(const char* unused){
    st_print_workspace();
}

void CLEAR(const char* unused){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void CLEAN(const char* unused){
    st_clean_vars();
    printf("Workspace cleaned 🧹");
}

void ECHO(const char* val){
    if (strcmp(val, "ON")) echo = TRUE;
    else if (strcmp(val, "OFF")) echo = FALSE;
    else echo = !echo;
}