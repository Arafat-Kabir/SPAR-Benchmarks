#ifndef FP_LIB_H
#define FP_LIB_H
#include <stdint.h>
typedef struct
{
    int64_t value;
    int int_bits; //bits for representing the integer not including the sign. Used for cutoff.
    int frac_bits; //bits for representing the fraction. Used for all operations.
} fpvar;

typedef int32_t fixp;
// //For right now, just assume that all fpvar's have matching int_bits and frac_bits;
// fpvar fp_add(fpvar a, fpvar b);
// fpvar fp_sub(fpvar a, fpvar b);
// fpvar fp_mul(fpvar a, fpvar b);

fixp fp_mul(fixp a, fixp b);
void float_to_fixed(void *input_double, int size, void *result_fp);
#endif