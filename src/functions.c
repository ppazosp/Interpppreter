#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "functions.h"
#include "symbol_table.h"
#include "constants.h"
#include "error_handler.h"
#include "stack.h"
#include "dll.c"

#define STACK_SIZE 8

Stack *stack;

Dll *libs;

FILE *current_fp = NULL;

int echo = TRUE;

extern void free_all();

void init_fncts(){
    stack = stack_create(STACK_SIZE);
    libs = dll_create();

    HELP("'unused");
    printf("\n\n");
}

void fncts_free(){
    dll_free(libs);
    stack_destroy(stack);
}

void* find_symbol(const char *sym){
    return dll_find_symbol(libs, sym);
}

void IMPORT(const char* libname){
    if(dll_contains(libs, libname)) {
        printf("%s is already imported", libname);
         return;
    }
 
    void *handle = dlopen(libname, RTLD_LAZY);
    if (!handle) {
        yyerror(dlerror());
        return;
    }

    dll_add(&libs, libname, handle);

    printf("Imported %s correctly", libs->name);
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
        free_all();
        exit(EXIT_SUCCESS);
    }
}

void HELP(const char* unused){
    printf("========= INTERPPPRETER HELP =========\n\n");
    printf("Welcome to Interpppreter! Here's what you can do:\n\n");
    printf("  LOAD(\"filename\")       - Load and execute commands from a file\n");
    printf("  QUIT()                 - Exit the interpreter or return to previous input\n");
    printf("  IMPORT(\"lib\")          - Dynamically import a shared library (.so or .dylib)\n");
    printf("  WORKSPACE()            - Show all variables, constants, and loaded libraries\n");
    printf("  CLEAR()                - Clear the screen\n");
    printf("  CLEAN()                - Remove all defined variables \n");
    printf("  ECHO(\"ON\"|\"OFF\")       - Enable or disable output echoing\n");
    printf("  HELP()                 - Show this help message\n\n");
    printf("Variables must be declared in lowercase.\n");
    printf("Constants must be declared in uppercase and cannot be reassigned.\n");
    printf("To remove both variables and constants, use CLEAN(\"FULL\").\n");
    printf("Functions must be called in lowercase.\n");
    printf("Commands must be called in uppercase.\n");
    printf("For commands without parameters, parentheses are optional.\n\n");
    printf("Have fun experimenting!\n");
    printf("\n=======================================");
}

void WORKSPACE(const char* unused){
    printf("\n======== WORKSPACE ========\n");
    printf(  "- Variables ---------------\n");
    st_print_items(IDENTIFIER);
    printf(  "- Constants ---------------\n");
    st_print_items(CONSTANT);
    printf(  "- Libraries ---------------\n");
    dll_print(libs);
    printf(  "===========================");
}

void CLEAR(const char* unused){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void CLEAN(const char* arg){
    int full = (strcmp(arg, "FULL") == 0); 

    st_clean_items(full);

    printf("Workspace cleaned succesfully");
}

void ECHO(const char* val){
    if ((strcmp(val, "ON")) == 0) echo = TRUE;
    else if ((strcmp(val, "OFF")) == 0) echo = FALSE;
    else echo = !echo;
}