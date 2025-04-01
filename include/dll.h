#ifndef DYNAMIC_LINK_LIST_H
#define DYNAMIC_LINK_LIST_H

typedef struct Dll {
    char *name;
    void *handle;
    struct Dll *next;
} Dll;

Dll* dll_create();
void dll_free(Dll *dll);
void dll_add(Dll **dll, const char *libname, void *handle);
int dll_contains(Dll *dll, const char *libname);
void dll_print(Dll *dll);
void* dll_find_symbol(Dll *dll, const char *func);

#endif // DYNAMIC_LINK_LIST_H