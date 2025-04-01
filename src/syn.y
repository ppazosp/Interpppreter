%{
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h> 

#include "symbol_table.h"
#include "error_handler.h"
#include "constants.h"
#include "functions.h"
#include "lex.yy.h"
#include "token.h"

extern void init_lex_parser(void);
extern int yylex_destroy(void);
extern int echo;

int show_value = TRUE;

%}

%define parse.error verbose

%union {
double val;
char* str;
}

%destructor { free($$); } <str>

%token GET LET

%token <val> NUM
%token <str> UP LW ARG
%type <val> exp

%right '='
%left GET LET
%left '<' '>'
%left '-' '+'
%left '*' '/'
%left NEG 
%right '^' 

%expect 3

%%

input:  
        | input line ;

line: 
        exp ';' { printf("\n"); } ;
        | exp { if(show_value && echo) { printf ("* %.10g", $1); } printf("\n\n"); fflush(stdout); }
        | error { yyclearin; yyerrok; printf("\n\n"); } ;

exp:    NUM { $$ = $1; }

        | LW { 
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == IDENTIFIER) {
                                $$ = token->value.var;

                                show_value = TRUE; 
                        } else if (token->id == FUNCTION) {
                                $$ = -1;
                                char buffer[64];
                                snprintf(buffer, sizeof(buffer), "Arguments missing for function %s", $1);
                                yyerror(buffer);

                                show_value = FALSE;  
                        }
                } else {
                        $$ = -1;
                        char buffer[64];
                        snprintf(buffer, sizeof(buffer), "%s is not defined", $1);
                        yyerror(buffer);

                        show_value = FALSE;
                }

                free($1);
        }
        
        | UP { 
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == CONSTANT) {
                                $$ = token->value.var;

                                show_value = TRUE; 
                        } else if (token->id == COMMAND) {
                                $$ = -1;

                                if(strcmp(token->key, "LOAD") == 0){
                                        char buffer[64];
                                        snprintf(buffer, sizeof(buffer), "Arguments missing for function %s", $1);
                                        yyerror(buffer);
                                }
                                else (*(token->value.cmdptr))("unused");
                                show_value = FALSE;  
                        }
                } else {
                        $$ = -1;
                        char buffer[64];
                        snprintf(buffer, sizeof(buffer), "%s is not defined", $1);
                        yyerror(buffer);

                        show_value = FALSE;
                }

                free($1);
        }

        | ARG {
                $$ = -1;

                printf("%s", $1);

                show_value = FALSE;

                free($1);
        }

        | LW '(' exp ')' { 
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == IDENTIFIER) {
                                $$ = token->value.var;

                                show_value = TRUE; 
                        } else if (token->id == FUNCTION) {
                                $$ = (*(token->value.fnctptr))($3);

                                show_value = TRUE;  
                        }
                } else {
                        fnctptr_t fptr = (double (*)(double)) find_symbol($1);

                        if (fptr != NULL) {
                                Token* token = malloc(sizeof(Token));
                                if (!token) {
                                        eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
                                        exit(EXIT_FAILURE);
                                }
                                token->key = strdup($1);
                                token->id = FUNCTION;
                                token->value.fnctptr = fptr;
                                st_insert(token);

                                $$ = fptr($3);
                                show_value = TRUE;
                        } else {
                                $$ = -1;
                                char buffer[64];
                                snprintf(buffer, sizeof(buffer), "%s is not defined", $1);
                                yyerror(buffer);

                                show_value = FALSE;
                        }
                }

                free($1);
        }

        | UP '(' ARG ')' { 
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == CONSTANT) {
                                $$ = token->value.var;

                                show_value = TRUE; 
                        } else if (token->id == COMMAND) {
                                $$ = -1;

                                (*(token->value.cmdptr))($3);

                                show_value = FALSE;  
                        }
                } else {
                        $$ = -1;
                        char buffer[64];
                        snprintf(buffer, sizeof(buffer), "%s is not defined", $1);
                        yyerror(buffer);

                        show_value = FALSE;
                }

                free($1);
                free($3);
        }

        | LW '=' exp {
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == IDENTIFIER) {
                                token->value.var = $3;
                                $$ = token->value.var;

                                show_value = TRUE; 

                        } else if (token->id == FUNCTION) {
                                $$ = -1;
                                char buffer[64];
                                snprintf(buffer, sizeof(buffer), "%s is a function, you cannot redefine a function", $1);
                                yyerror(buffer);

                                show_value = FALSE;  
                        }
                } else {
                        token = malloc(sizeof(Token));
                        if(!token){
                                eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
                                exit(EXIT_FAILURE);
                        }
                        token->key = strdup($1);
                        token->id = IDENTIFIER;
                        token->value = (union Value) { .var = $3 };
                        st_insert(token);

                        $$ = token->value.var;

                        show_value = TRUE;
                }

                free($1);
        }

        | UP '=' exp {
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == CONSTANT) {
                                $$ = -1;
                                char buffer[64];
                                snprintf(buffer, sizeof(buffer), "%s is a constant, you cannot redefine a constant", $1);
                                yyerror(buffer);

                                show_value = FALSE; 

                        } else if (token-> id == COMMAND){
                                $$ = -1;
                                char buffer[64];
                                snprintf(buffer, sizeof(buffer), "%s is a command, you cannot redefine a command", $1);
                                yyerror(buffer);

                                show_value = FALSE; 
                        }
                } else {
                        token = malloc(sizeof(Token));
                        if(!token){
                                eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
                                exit(EXIT_FAILURE);
                        }
                        token->key = strdup($1);
                        token->id = CONSTANT;
                        token->value = (union Value) { .var = $3 };

                        st_insert(token);

                        $$ = token->value.var;

                        show_value = TRUE;
                }

                free($1);
        }

        | exp '+' exp { $$ = $1 + $3; show_value = TRUE; }
        | exp '-' exp { $$ = $1 - $3; show_value = TRUE; }
        | exp '*' exp { $$ = $1 * $3; show_value = TRUE; }
        | exp '/' exp { $$ = $1 / $3; show_value = TRUE; }
        | exp '<' exp { $$ = $1 < $3; show_value = TRUE; }
        | exp '>' exp { $$ = $1 > $3; show_value = TRUE; }
        | exp GET exp { $$ = $1 >= $3; show_value = TRUE; }
        | exp LET exp { $$ = $1 <= $3; show_value = TRUE; }
        | '-' exp %prec NEG { $$ = -$2; show_value = TRUE; }
        | exp '^' exp { $$ = pow ($1, $3); show_value = TRUE; }
        | '(' exp ')' { $$ = $2; show_value = TRUE; };

%%

void syn_parser_free(void){
    yylex_destroy();
}