#include "utils.h"
#include <stdio.h>
#include <assert.h>



// Prints a mnist_record
void printDataRecord(const mnist_record *item, bool show_vector) {
  // print label indices
  printf("label_index: %d, predicted_index: %d\n", item->label_index, item->predicted_index);

  // optionally print the feature-vector
  if(show_vector) {
    int feature_len = 784;
    for(int i=0; i<feature_len; ++i) {
      printf("%f, ", item->feature_vec[i]);
    }
    printf("\n");
  }
}


// Givne mlp12_Params_fl, prints the metadata fields
void printMLP12meta(const mlp12_Params_fl *params) {
  printf("accuracy  : %.2f%%\n", params->accuracy);
  printf("summary   : %s\n", params->summary);
  printf("fc1_weight: (%d, %d)\n", params->fc1_weight.row_cnt, params->fc1_weight.col_cnt);
  printf("fc2_weight: (%d, %d)\n", params->fc2_weight.row_cnt, params->fc2_weight.col_cnt);
}


// ---- Math Utilities ----

// Given two float arrays, adds them element-wise
void add_vec(const float *vec_a, const float *vec_b, float *vec_out, int vec_len) {
  for(int i=0; i<vec_len; ++i) 
    vec_out[i] = vec_a[i] + vec_b[i];
}


// Given a float array, performs relu on it
void apply_relu_vec(const float *vec_in, float *vec_out, int vec_len) {
  for(int i=0; i<vec_len; ++i) 
    vec_out[i] = (vec_in[i] > 0) ? vec_in[i] : 0;
}


// Given a float array, returns the index of the max value
int argmax_fl(const float *vec_in, int vec_len) {
  int   max_index = 0;
  float max_val = vec_in[0];

  // Find the max value and index
  for(int i=1; i<vec_len; ++i) {
    if(vec_in[i] > vec_in[max_index]) {
      max_index = i;
      max_val   = vec_in[i];
    }
  }

  return max_index;
}


// Given a 2D float array and a 1D float array, performs matrix-vector multiplication
// row_cnt, col_cnt: size of the mat array
// col_cnt: length of the vec and out_vec arrays
void mat_vec_mult(const float *mat, const float *vec, float *out_vec, int row_cnt, int col_cnt) {
  // Multiply row-wise, then accumulate in out_vec
  for(int r=0; r<row_cnt; ++r) {
    out_vec[r] = 0.0f;    // zero out for accumulation
    for(int c=0; c<col_cnt; ++c) {
      out_vec[r] += mat[r*col_cnt + c] * vec[c];
    }
  }
}





// ---- Unit tests ----
void test_add_vec() {
  printf("INFO: Running test_add_vec() ... ");

  // define test inputs and reference output
  float vec_a[]   = {0,1,2,-1,-2};
  float vec_b[]   = {1,1,1,1,1};
  float vec_ref[] = {1,2,3,0,-1};
  float vec_o[5];
  const int vec_len = 5;

  // run through function
  add_vec(vec_a, vec_b, vec_o, vec_len);

  for(int i=0; i<vec_len; ++i) {
    assert(("Problem with add_vec utility", vec_o[i] == vec_ref[i]));
  }

  printf("Passed!\n");
}


void test_apply_relu_vec() {
  printf("INFO: Running test_apply_relu_vec() ... ");

  // define test inputs and reference output
  float vec_in[]   = {0,1,2,-1,-2};
  float vec_ref[] = {0,1,2,0,0};
  float vec_o[5];
  const int vec_len = 5;

  // run through function
  apply_relu_vec(vec_in, vec_o, vec_len);

  for(int i=0; i<vec_len; ++i) {
    assert(("Problem with apply_relu_vec utility", vec_o[i] == vec_ref[i]));
  }

  printf("Passed!\n");
}


void test_argmax_fl() {
  printf("INFO: Running test_argmax_fl() ... ");

  // define test inputs and reference output
  float vec_in[] = {0,1,30,-1,-2};
  const int vec_len = 5;
  int   out_ref  = 2;

  // run through function
  int out = argmax_fl(vec_in, vec_len);

  assert(("Problem with argmax_fl utility", out == out_ref));

  printf("Passed!\n");
}


void test_mat_vec_mult() {
  printf("INFO: Running test_mat_vec_mult() ... ");
  fflush(stdout);

  // define test inputs and reference output
  float mat[2][3]  = { 1.0f, 2.0f, 3.0f, 
                       4.0f, 5.0f, 6.0f  };
  float vec[3]     = { 2.0f, 3.0f, 4.0f };
  float ref_vec[2] = {20.0, 47.0};
  float out_vec[2];
  int row_cnt = 2, col_cnt = 3;

  // run through the function under test
  mat_vec_mult((float*)mat, vec, out_vec, row_cnt, col_cnt);

  // validate
  for(int i=0; i<row_cnt; ++i) {
    assert(("Problem with mat_vec_mult utility", out_vec[i] == ref_vec[i]));
  }

  printf("Passed!\n");
}

