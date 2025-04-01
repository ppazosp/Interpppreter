#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "dll.h"
#include "error_handler.h"
#include "constants.h"

Dll* dll_create(){
    return NULL;
}

void dll_free(Dll* dll) {
    Dll *current = dll;
    while (current != NULL) {
        Dll *next = current->next;
        if (current->handle) {
            dlclose(current->handle);
        }
        free(current->name);
        free(current);
        current = next;
    }
    dll = NULL;
}

void dll_add(Dll** dll, const char* libname, void* handle){
    Dll *new_dll = malloc(sizeof(Dll));
    new_dll->name = strdup(libname);
    new_dll->handle = handle;
    new_dll->next = *dll;
    *dll = new_dll;
}

int dll_contains(Dll* dll, const char* libname){
    for (Dll *aux = dll; aux != NULL; aux = aux->next) {
        if (strcmp(aux->name, libname) == 0) return TRUE;
    }
    return FALSE;
}

void* dll_find_symbol(Dll* dll, const char *func) {
    for (Dll *aux = dll; aux != NULL; aux = aux->next) {
        void *f = dlsym(aux->handle, func);
        if (f) return f;
    }
    return NULL;
}

void dll_print(Dll* dll ) {
    Dll *aux = dll;
    while (aux != NULL) {
        printf("| %-21s |\n", aux->name);
        aux = aux->next;
    }
}