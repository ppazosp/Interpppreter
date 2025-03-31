%{
#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "syn.tab.h"
#include "error_handler.h"
#include "constants.h"

static char id_holder = 0;

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

LOWER [a-z]
UPPER [A-Z]

CONSTANT {UPPER}({NUMERIC_LITERAL}|{UPPER})*
IDENTIFIER {LOWER}({NUMERIC_LITERAL}|{LOWER})*


SYMBOL [+\-*/()^<>=]

GET_SYMBOL ">='"

LET_SYMBOL "<="

CARRIAGE [\n]

%%

{NUMERIC_LITERAL}               {   yylval.val = atof(yytext); return NUM; }

{IDENTIFIER}                    {   yylval.str = strdup(yytext); return LW; }

{CONSTANT}                      {   yylval.str = strdup(yytext); return UP; }

{GET_SYMBOL}                    {   return GET; }

{LET_SYMBOL}                    {   return LET; }                                

{SYMBOL}                        {   return (int) yytext[0]; }

{SPC}+                          {   }

{CARRIAGE}                      {   return '\n'; }

%%

void init_lex_parsing(void){
    _init_input_system();
}
