// header guard

#include <stdio.h>
#include <stdlib.h>
typedef int reg;
typedef double* mat;
typedef double* vec;
typedef int acttype;

void load_m(reg a, void * b, int rows, int cols);
void load_v(reg a, vec b, int size);
void load_v_t(reg a, vec b, int size);
void printreg(reg a);
void printreg_v(reg a);
void printreg_segment(reg a, int row, int col);
void printreg_to_file(reg a, int row, int col, char *filename);
void e_mul_mv(reg m, reg v, int rowm, int colm, reg result);
void acc_col(reg m, int rowm, int colm, reg temp, reg result);
void add(reg a, reg b, reg des);
void e_mul(reg a, reg b, reg result);
void ReLU(reg a, reg des);
void activation(reg a, reg des, acttype act);
void rotate(reg a);
void rotate_mov(reg a, reg des);
void store(reg a, vec b);

void lstm_gate(reg x, reg Wi, reg Bi, reg h, reg Wh, reg Bh, int x_size, int h_size, int output_size, int act_func, reg temp1, reg temp2, reg temp3, reg des);

//just for testing
void write_array_to_file(FILE *outfile, void *array, long size, char *name, int end);
