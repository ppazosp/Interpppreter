#ifndef TOKEN_H
#define TOKEN_H

typedef double (*fnctptr_t)(double);

typedef union Value{
    double var;
    fnctptr_t fnctptr;
} Value;

typedef struct Token {
    char* key;
    int id;
    Value value;
} Token;

#endif // TOKEN_H