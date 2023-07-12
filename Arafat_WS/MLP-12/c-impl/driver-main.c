// This driver is written to test the model and dataset.
// Some utilities are deveoped, which are also test here.

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mnist_dataset.h"
#include "mlp12_params.h"
#include "utils.h"


void run_tests() {
  test_add_vec();
  test_apply_relu_vec();
  test_argmax_fl();
  test_mat_vec_mult();
}


// Performs: vec_out = weights @ vec_in + bias
void mlp12_linear(const float *weight, const float *vec_in, const float *bias, float *vec_out, int row_cnt, int col_cnt) {
  float temp_vec[20];   // len = 12 would be enough for MLP-12

  // temp_vec = params.fc1_weight @ feature_vec
  mat_vec_mult(weight, vec_in,     // input mat/vecs
               temp_vec,           // output vector
               row_cnt, col_cnt);  // dimensions
  // vec_out = temp_vec + bias
  add_vec(temp_vec, bias,   // input vectors
          vec_out,          // output vector
          row_cnt);         // dimensions
}


/**** Implements the forward function of the MLP-12 model
* def mlp12_forward(params, feature_vec):
*     x1 = params.fc1_weight @ feature_vec + params.fc1_bias
*     fc1_out = npReLU(x1)
*     fc2_out = params.fc2_weight @ fc1_out + params.fc2_bias
*     return fc2_out
****/
void mlp12_forward(const mlp12_Params_fl *params, const float *feature_vec, float *out_vec) {
  const int feat_len = 784;
  float temp_vec1[20], temp_vec2[20];   // len = 12 would be enough

  // -- forward computation --
  // temp_vec1 = params.fc1_weight @ feature_vec + params.fc1_bias
  mlp12_linear(params->fc1_weight.elements,             // input weights
               feature_vec, params->fc1_bias.elements,  // input vector and bias
               temp_vec1,                               // output vector
               params->fc1_weight.row_cnt, params->fc1_weight.col_cnt);  // dimensions
                                                                         
  // temp_vec2 = relu(temp_vec1)                                                                    
  apply_relu_vec(temp_vec1,             // input vector
                 temp_vec2,             // output vector
                 params->fc1_weight.row_cnt);  // dimension

  // out_vec = params.fc2_weight @ fc1_out + params.fc2_bias;   fc1_out = temp_vec2
  mlp12_linear(params->fc2_weight.elements,            // input weights
               temp_vec2, params->fc2_bias.elements,   // input vector and bias
               out_vec,                                // output vector
               params->fc2_weight.row_cnt, params->fc2_weight.col_cnt);  // dimensions
}



// Given a feature vector, returns the index prediction
int mlp12_predict(const mlp12_Params_fl *params, const float *feature_vec) {
  float out_vec[20];   // len = 10 would be enough
  int out_len = params->fc2_weight.row_cnt;   // get the len of model output
  mlp12_forward(params, feature_vec, out_vec);
  return argmax_fl(out_vec, out_len);
}




int main() {
  //run_tests();

  printf("MLP-12 model metadata:\n");
  printMLP12meta(&mlp12_trained_params);
  printf("\n");

  const mnist_record  *item;
  item = &mnist_dataset[0];


  // loop over dataset and run prediction
  int pred, total=0, exp_miss=0, correct=0;
  for(int i=0; i<mnist_DATASET_LEN; ++i) {
  //for(int i=0; i<3; ++i) {
    // printDataRecord(item, false);
    pred = mlp12_predict(&mlp12_trained_params, item->feature_vec);
    // printf("pred: %d\n", pred);

    // record stats
    if(pred == item->label_index)     correct += 1;
    if(pred != item->predicted_index) exp_miss += 1;
    total += 1;

    ++item;
  }
  
  // print status
  float accuracy = (correct * 100.0) / total;
  printf("Prediction stats:\n");
  printf("  accuracy: %.2f%%\n", accuracy);
  printf("  total   : %d\n", total);
  printf("  correct : %d\n", correct);
  printf("  exp_miss: %d\n", exp_miss);
  return 0;
}
