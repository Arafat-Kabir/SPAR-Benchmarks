#include "fplib.h"
#include <stdio.h>

fpvar fp_add(fpvar a, fpvar b){
    fpvar c;
    c.value = a.value + b.value;
    printf("C: %llX\n", c.value);
    c.frac_bits = a.frac_bits;
    c.int_bits = a.int_bits;
    // todo. Handle overload more properly
    int64_t mask = -1;
    int64_t negone = -1;
    if(c.value>0){
    // printf("Mask: %llX\n", mask);
    mask = mask<<(a.frac_bits+a.int_bits);
    // printf("Mask: %llX\n", mask);
    mask = mask ^ ((int64_t)1<<63);
    // printf("Mask: %llX\n", mask);
    mask = mask ^ (negone);
    // printf("Mask: %llX\n", mask);
    c.value &= mask;
    }
    else{
        mask = mask<<(a.frac_bits+a.int_bits);
        c.value |= mask;
    }

    return c;
}

fpvar fp_sub(fpvar a, fpvar b){
    fpvar c;
    c.value = a.value-b.value;
    c.frac_bits = a.frac_bits;
    c.int_bits = a.int_bits;
    // todo. Handle overload more properly
    int64_t mask = -1;
    int64_t negone = -1;
    if(c.value>0){
    // printf("Mask: %llX\n", mask);
    mask = mask<<(a.frac_bits+a.int_bits);
    // printf("Mask: %llX\n", mask);
    mask = mask ^ ((int64_t)1<<63);
    // printf("Mask: %llX\n", mask);
    mask = mask ^ (negone);
    // printf("Mask: %llX\n", mask);
    c.value &= mask;
    }
    else{
        mask = mask<<(a.frac_bits+a.int_bits);
        c.value |= mask;
    }
    return c;
}

fpvar fp_mul(fpvar a, fpvar b){
    fpvar c;
    c.value = a.value*b.value;
    c.frac_bits = a.frac_bits;
    c.int_bits = a.int_bits;
    c.value = (c.value>>c.frac_bits);
    // todo. Handle overload more properly
    int64_t mask = -1;
    int64_t negone = -1;
    if(c.value>0){
    // printf("Mask: %llX\n", mask);
    mask = mask<<(a.frac_bits+a.int_bits);
    // printf("Mask: %llX\n", mask);
    mask = mask ^ ((int64_t)1<<63);
    // printf("Mask: %llX\n", mask);
    mask = mask ^ (negone);
    // printf("Mask: %llX\n", mask);
    c.value &= mask;
    }
    else{
        mask = mask<<(a.frac_bits+a.int_bits);
        c.value |= mask;
    }
    return c;
}