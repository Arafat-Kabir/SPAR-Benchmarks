#include "fnlib.h"
#include <stdio.h>

//call spar functions here later

void load(reg a, mat b) { 
    printf("load from driver\r\n");
}


void mm(reg a, mat b) { 
    printf("mm from driver\r\n");
}

void add(reg a, reg b) {
    printf("add from driver\r\n");
}

void activation(reg a, acttype act) {
    printf("activation from driver\r\n");
}

void rotate(reg a) {
    printf("rotate from driver\r\n");
}

void store(reg a, vec b) {
    printf("store from driver\r\n");
}
