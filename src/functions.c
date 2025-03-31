#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "symbol_table.h"

void LOAD(char* unused){
    printf("LOAD");
}

void QUIT(char* unused){
    printf("QUIT");
}

void HELP(char* unused){
    printf("HELP");
}

void WORKSPACE(char* unused){
    st_print_vars();
}

void CLEAR(char* unused){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void CLEAN(char* unused){
    st_clean_vars();
    printf("Workspace cleaned 🧹");
}

void ECHO(char* unused){
    printf("ECHO");
}