%{
#include <stdio.h>  
#include <math.h> 
#include "symbol_table.h"
#include "error_handler.h"
#include "constants.h"

int show_value = TRUE;

extern int yylex(void);
%}

%union {
double val;
Token *tptr; 
}

%token GET LET

%token <val> NUM 
%token <tptr> VAR CONS FNCT 
%type <val> exp

%right '='
%left GET LET
%left '<' '>'
%left '-' '+'
%left '*' '/'
%left NEG 
%right '^' 

%%

input:  prompt
        | input line ;

line:   prompt '\n'
        | prompt exp '\n' { if(show_value) { printf ("%.10g", $2); } printf("\n\n> "); fflush(stdout); }
        | prompt error '\n' { yyclearin; yyerrok; printf("\n> ")} ;

prompt: { printf("> "); fflush(stdout); } ;

exp: NUM { $$ = $1; }
        | VAR { $$ = $1->value.var; }
        | CONS { $$ = $1->value.var; }
        | VAR '=' exp { $$ = $3; $1->value.var = $3; }
        | CONS '=' exp { $$ = $1->value.var; if ($1->value.var == 0){ $1->value.var = $3; $$ = $3; }}
        | FNCT '(' exp ')' { $$ = (*($1->value.fnctptr))($3); show_value = TRUE }
        | FNCT '('')' { $$ = (*($1->value.fnctptr))(-1); show_value = FALSE }
        | exp '+' exp { $$ = $1 + $3; }
        | exp '-' exp { $$ = $1 - $3; }
        | exp '*' exp { $$ = $1 * $3; }
        | exp '/' exp { $$ = $1 / $3; }
        | exp '<' exp { $$ = $1 < $3; }
        | exp '>' exp { $$ = $1 > $3; }
        | exp GET exp { $$ = $1 >= $3; }
        | exp LET exp { $$ = $1 <= $3; }
        | '-' exp %prec NEG { $$ = -$2; }
        | exp '^' exp { $$ = pow ($1, $3); }
        | '(' exp ')' { $$ = $2; } ;

%%