#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

double LOAD(double unused){
    printf("LOAD");
    return 1;
}

double QUIT(double unused){
    printf("QUIT");
    return 1;
}

double HELP(double unused){
    printf("HELP");
    return 1;
}

double WORKSPACE(double unused){
    printf("WORKSPACE");
    return 1;
}

double CLEAR(double unused){
    printf("CLEAR");
    return 1;
}

double CLEAN(double unused){
    printf("CLEAN");
    return 1;
}

double ECHO(double unused){
    printf("ECHO");
    return 1;
}