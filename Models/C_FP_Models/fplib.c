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

// fpvar fp_mul(fpvar a, fpvar b){
//     fpvar c;
//     c.value = a.value*b.value;
//     c.frac_bits = a.frac_bits;
//     c.int_bits = a.int_bits;
//     c.value = (c.value>>c.frac_bits);
//     // todo. Handle overload more properly
//     int64_t mask = -1;
//     int64_t negone = -1;
//     if(c.value>0){
//     // printf("Mask: %llX\n", mask);
//     mask = mask<<(a.frac_bits+a.int_bits);
//     // printf("Mask: %llX\n", mask);
//     mask = mask ^ ((int64_t)1<<63);
//     // printf("Mask: %llX\n", mask);
//     mask = mask ^ (negone);
//     // printf("Mask: %llX\n", mask);
//     c.value &= mask;
//     }
//     else{
//         mask = mask<<(a.frac_bits+a.int_bits);
//         c.value |= mask;
//     }
//     return c;
// }

fixp fp_mul(fixp a, fixp b){
    int64_t aa = (int64_t)a;
    int64_t bb = (int64_t)b;
    int64_t cc = aa*bb;
    cc = cc>>16;
    fixp c = (fixp)cc;
    return c;
}

void float_to_fixed(double *a, int size, fixp *result){
    for(int i=0; i<size; i++){
        result[i] = (fixp)(a[i]*((double)(1<<15)));
    }
    return;
}