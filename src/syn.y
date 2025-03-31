%{
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <math.h> 

#include "symbol_table.h"
#include "error_handler.h"
#include "constants.h"
#include "stack.h"

int show_value = TRUE;

extern int yylex(void);
extern void yyrestart(FILE *input_file);
extern FILE *yyin;
extern void init_lex_parsing(void);

#define STACK_SIZE 8

Stack* fp_stack;

void _init_fp_stack(){
        fp_stack = stack_create(STACK_SIZE);
}

void load(const char* filename){
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "Could not open file %s", filename);
                yyerror(buffer);
        }

        stack_push(fp_stack, yyin);
        yyin = fp;
}

void quit(void){
        if(stack_is_empty(fp_stack)) {
                st_print();
                st_free();
        }
        else {
                yyin = stack_pop(fp_stack);
                yyrestart(yyin);
                printf("Quitting\n\n");
                ungetc('\n', yyin);
        }
}

%}

%define parse.error verbose

%union {
double val;
char* str;
}

%token GET LET
%token QUIT

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

%%

input:  prompt
        | input line ;

line:   prompt '\n'
        | prompt exp '\n' { if(show_value) { printf ("%.10g", $2); } printf("\n\n> "); fflush(stdout);}
        | prompt error '\n' { yyclearin; yyerrok; printf("\n> "); } ;
        | prompt exp ';' '\n' { }
        | prompt QUIT { quit(); };

prompt: { printf("> "); fflush(stdout); } ;

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
        }
        
        | UP { 
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == CONSTANT) {
                                $$ = token->value.var;

                                show_value = TRUE; 
                        } else if (token->id == COMMAND) {
                                $$ = -1;

                                if(strcmp(token->key, "QUIT") == 0){ quit(); }
                                else if(strcmp(token->key, "LOAD") == 0){
                                        char buffer[64];
                                        snprintf(buffer, sizeof(buffer), "Arguments missing for function %s", $1);
                                        yyerror(buffer);
                                }
                                else (*(token->value.cmdptr))(NULL);
                                show_value = FALSE;  
                        }
                } else {
                        $$ = -1;
                        char buffer[64];
                        snprintf(buffer, sizeof(buffer), "%s is not defined", $1);
                        yyerror(buffer);

                        show_value = FALSE;
                }
        }

        | UP '(' ARG ')' { 
                Token* token = st_search($1);
                if (token != NULL){
                        if (token->id == CONSTANT) {
                                $$ = token->value.var;

                                show_value = TRUE; 
                        } else if (token->id == COMMAND) {
                                $$ = -1;

                                if(strcmp(token->key, "LOAD") == 0) load($3);
                                else if(strcmp(token->key, "QUIT") == 0) quit();
                                else(*(token->value.cmdptr))(NULL);

                                show_value = FALSE;  
                        }
                } else {
                        $$ = -1;
                        char buffer[64];
                        snprintf(buffer, sizeof(buffer), "%s is not defined", $1);
                        yyerror(buffer);

                        show_value = FALSE;
                }
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

void init_syn_parsing(void){
    _init_fp_stack();
    init_lex_parsing();
    yyparse();
}