#include <stdio.h>
#include <stdlib.h>

#include "error_handler.h"

int warn = 1;

void eh_printerr(const char* message, int mode, int line){
    switch(mode){
        case 0:
            if(warn)fprintf(stderr, "WARNING in line %d: %s\n\n", line, message);
            break;

        case 1:
            fprintf(stderr, "LEXICAL ERROR in line %d: %s\n\n", line, message);
            break;
        case 2:
        fprintf(stderr, "FATAL ERROR in line %d: %s\n\n", line, message);
        break;
    }
}

void set_warn(int value){
    warn = value;
}

void yyerror(const char* s){
    fprintf(stderr, "%s", s);
}
