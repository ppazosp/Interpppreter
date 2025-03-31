#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#define WARNING 0
#define LEXICAL_ERROR 1
#define FATAL_ERROR 2

void eh_printerr(const char* error, int mode, int line);

void set_warn(int value);

void yyerror (const char* s);

#endif // ERROR_HANDLER_H
