#ifndef UTILS_H
#define UTILS_h


#include <stdbool.h>
#include "mnist_dataset.h"
#include "mlp12_params.h"


void printDataRecord(const mnist_record *item, bool show_vector);
void printMLP12meta(const mlp12_Params_fl *params);

void add_vec(const float *vec_a, const float *vec_b, float *vec_out, int vec_len);
void apply_relu_vec(const float *vec_in, float *vec_out, int vec_len);
int  argmax_fl(const float *vec_in, int vec_len);
void mat_vec_mult(const float *mat, const float *vec, float *out_vec, int row_cnt, int col_cnt);


// Unit-test functions
void test_add_vec();
void test_apply_relu_vec();
void test_argmax_fl();
void test_mat_vec_mult();


#endif  // UTILS_H
