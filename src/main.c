#include <stdio.h>  
#include <stdlib.h> 
#include <math.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "constants.h"
#include "symbol_table.h"
#include "string.h"
#include "error_handler.h"
#include "functions.h"
#include "lex.yy.h"
#include "stack.h"

extern int yyparse(void);
extern void syn_parser_free(void);



#define KEYWORDS_COUNT 16

Token** create_keywords(void) {
    static Token* keywords[KEYWORDS_COUNT] = { NULL };

    if (keywords[0] == NULL) {
        keywords[0] = malloc(sizeof(Token));
        if (!keywords[0]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[0]->key = strdup("sin");
        keywords[0]->id = FUNCTION;
        keywords[0]->value.fnctptr = sin;

        keywords[1] = malloc(sizeof(Token));
        if (!keywords[1]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[1]->key = strdup("cos");
        keywords[1]->id = FUNCTION;
        keywords[1]->value.fnctptr = cos;

        keywords[2] = malloc(sizeof(Token));
        if (!keywords[2]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[2]->key = strdup("tan");
        keywords[2]->id = FUNCTION;
        keywords[2]->value.fnctptr = tan;

        keywords[3] = malloc(sizeof(Token));
        if (!keywords[3]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[3]->key = strdup("log");
        keywords[3]->id = FUNCTION;
        keywords[3]->value.fnctptr = log;

        keywords[4] = malloc(sizeof(Token));
        if (!keywords[4]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[4]->key = strdup("exp");
        keywords[4]->id = FUNCTION;
        keywords[4]->value.fnctptr = exp;

        keywords[5] = malloc(sizeof(Token));
        if (!keywords[5]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[5]->key = strdup("sqrt");
        keywords[5]->id = FUNCTION;
        keywords[5]->value.fnctptr = sqrt;

        /////////////////////////////////////////////////////

        keywords[6] = malloc(sizeof(Token));
        if (!keywords[6]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[6]->key = strdup("E");
        keywords[6]->id = CONSTANT;
        keywords[6]->value.var = 2.7182;

        keywords[7] = malloc(sizeof(Token));
        if (!keywords[7]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[7]->key = strdup("PI");
        keywords[7]->id = CONSTANT;
        keywords[7]->value.var = 3.1416;

        /////////////////////////////////////////////////////

        keywords[8] = malloc(sizeof(Token));
        if (!keywords[8]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[8]->key = strdup("LOAD");
        keywords[8]->id = COMMAND;
        keywords[8]->value.cmdptr = LOAD;

        keywords[9] = malloc(sizeof(Token));
        if (!keywords[9]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[9]->key = strdup("QUIT");
        keywords[9]->id = COMMAND;
        keywords[9]->value.cmdptr = QUIT;

        keywords[10] = malloc(sizeof(Token));
        if (!keywords[10]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[10]->key = strdup("HELP");
        keywords[10]->id = COMMAND;
        keywords[10]->value.cmdptr = HELP;

        keywords[11] = malloc(sizeof(Token));
        if (!keywords[11]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[11]->key = strdup("WORKSPACE");
        keywords[11]->id = COMMAND;
        keywords[11]->value.cmdptr = WORKSPACE;

        keywords[12] = malloc(sizeof(Token));
        if (!keywords[12]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[12]->key = strdup("CLEAR");
        keywords[12]->id = COMMAND;
        keywords[12]->value.cmdptr = CLEAR;

        keywords[13] = malloc(sizeof(Token));
        if (!keywords[13]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[13]->key = strdup("CLEAN");
        keywords[13]->id = COMMAND;
        keywords[13]->value.cmdptr = CLEAN;

        keywords[14] = malloc(sizeof(Token));
        if (!keywords[14]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[14]->key = strdup("ECHO");
        keywords[14]->id = COMMAND;
        keywords[14]->value.cmdptr = ECHO;

        keywords[15] = malloc(sizeof(Token));
        if (!keywords[15]) {
            eh_printerr("Memory allocation error\n", FATAL_ERROR, 0);
            exit(EXIT_FAILURE);
        }
        keywords[15]->key = strdup("IMPORT");
        keywords[15]->id = COMMAND;
        keywords[15]->value.cmdptr = IMPORT;
    }
    return keywords;
}

char *get_next_line(void) {
    char *line;

    if (current_fp != NULL) {

        char buffer[64];
        if (fgets(buffer, sizeof(buffer), current_fp)) {
            line = strdup(buffer);
        } else {
            QUIT("unused");
            
            line = strdup("\n");
        }
    } else {
        line = readline("> ");
    }
    return line;
}

void repl_loop(void) {
    using_history();

    while (1) {
        char *line = get_next_line();

        if (current_fp == NULL && *line != '\0') {
            add_history(line);
        }


        YY_BUFFER_STATE bp = yy_scan_string(line);
        yyparse();
        yy_delete_buffer(bp);

        free(line);
    }
}

void free_all(){
    fncts_free();
    syn_parser_free();
    st_free();
    clear_history();        
    rl_deprep_terminal();    
    rl_cleanup_after_signal();
}

int main(void){

    if (st_init(create_keywords(), KEYWORDS_COUNT) < 0) {
        printf("ERROR INITIALIZING SYMBOL TABLE");
        return EXIT_FAILURE;
    }
    
    st_print();

    init_fncts();

    repl_loop();

    return EXIT_SUCCESS;
}
