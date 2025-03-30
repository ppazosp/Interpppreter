%{
#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "syn.tab.h"
#include "error_handler.h"
#include "constants.h"

static char id_holder = 0;

Token* _search_st(const char* key, int id, Value value) {
    return st_search(key, id, value);
}

void _init_input_system(void){
    yylineno = 1;
}

%}

%option noyywrap nounput noinput yylineno

DECIMAL [0-9]

INTEGER_LITERAL {DECIMAL}+

FLOAT_LITERAL ({INTEGER_LITERAL}"."{DECIMAL}*)|("."{DECIMAL}+)

NUMERIC_LITERAL {INTEGER_LITERAL}|{FLOAT_LITERAL}

SPC [ \t\r]

MINUS [a-z]
MAYUS [A-Z]

CONSTANT {MAYUS}({NUMERIC_LITERAL}|{MAYUS})*
IDENTIFIER {MINUS}({NUMERIC_LITERAL}|{MINUS})*

SYMBOL [+\-*/()^<>=]

GET_SYMBOL ">='"

LET_SYMBOL "<="

CARRIAGE [\n]

%%

{NUMERIC_LITERAL}               {   yylval.val = atof(yytext);
                                    return NUM; }

{GET_SYMBOL}                           {   return GET; }

{LET_SYMBOL}                           {   return LET; }

{SYMBOL}                        {   return (int) yytext[0]; }

{IDENTIFIER}                    {   Value value;
                                    value.var = 0;
                                    value.fnctptr = NULL;
                                    Token* t = _search_st(yytext, IDENTIFIER, value);
                                    yylval.tptr = t;
                                    if(t->id == FUNCTION){ return FNCT; }
                                    else{ return VAR; }
                                }

{CONSTANT}                      {   Value value;
                                    value.var = 0;
                                    value.fnctptr = NULL;
                                    Token* t = _search_st(yytext, CONSTANT, value);
                                    yylval.tptr = t;
                                    if(t->id == FUNCTION){ return FNCT; }
                                    else{ return CONSTANT; }
                                }

{SPC}+                          {   }

{CARRIAGE}                      { return '\n'; }

%%

void init_lex_parsing(void){
    _init_input_system();
}
