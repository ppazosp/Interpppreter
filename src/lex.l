%{
#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "syn.tab.h"
#include "error_handler.h"
#include "constants.h"

#define STACK_SIZE 8

%}

%option noyywrap nounput noinput

%x QUOTED

DECIMAL [0-9]

INTEGER_LITERAL {DECIMAL}+

FLOAT_LITERAL ({INTEGER_LITERAL}"."{DECIMAL}*)|("."{DECIMAL}+)

NUMERIC_LITERAL {INTEGER_LITERAL}|{FLOAT_LITERAL}

SPC [ \t\r]

LOWER [a-z]
UPPER [A-Z]

ALPHA ({LOWER}|{UPPER})+

CONSTANT {UPPER}({NUMERIC_LITERAL}|{UPPER})*
IDENTIFIER {LOWER}({NUMERIC_LITERAL}|{LOWER})* 

SYMBOL [+\-*/()^<>=;]

GET_SYMBOL ">='"

LET_SYMBOL "<="

QUOTE \"

%%

{NUMERIC_LITERAL}               {   yylval.val = atof(yytext); return NUM; }

{QUOTE}                         {   BEGIN(QUOTED); }

<QUOTED>[^"]+                   {   yylval.str = strdup(yytext); }

<QUOTED>{QUOTE}                 {   BEGIN(INITIAL); return ARG; }

{IDENTIFIER}                    {   yylval.str = strdup(yytext); return LW; }

{CONSTANT}                      {   yylval.str = strdup(yytext); return UP; }

{GET_SYMBOL}                    {   return GET; }

{LET_SYMBOL}                    {   return LET; }                                

{SYMBOL}                        {   return (int) yytext[0]; }

{SPC}+                          {   }

\n                              {   }

.                               {   }

%%