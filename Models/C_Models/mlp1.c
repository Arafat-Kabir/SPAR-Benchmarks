#include "mlp1.h"
#include "fnlib.h"
#include <stdio.h> 

#define inputSize 78//784
#define m1 10
#define m2 10 //output

static float input[inputSize];
static float w1[m1][inputSize];//[rows][cols]
static float bias1[m1];

void initialize(void *param) {
    // initialize parameters
    for(int i=0; i<inputSize; i++)
    {
        input[i] = 1.0f;
    }
    for(int i=0; i<m1; i++)
    {
        for(int j=0; j<inputSize; j++)
        {
            w1[i][j] = (float)i;
            // printf("%.2f, ", w1[i][j]);
        }
        // printf("\n");
    }
    for(int i=0; i<m1; i++)
    {
        bias1[i]=-10.0f;
    }
}



int run_inference() { //change this
    initialize(NULL);

    //first layer
    load_v(1, input, inputSize);
    load_m(2, w1, m1, inputSize);
    e_mul_mv(2, 1, m1, inputSize, 3);
    acc_col(3, m1, inputSize, 0, 4); //accumulated vector in reg4
    load_v_t(3, bias1, m1); //load bias in turned orientation to 
    add(3, 4, 4); //add bias to vector
    ReLU(4, 4);
    // rotate
    // mm(a,b);
    // add(a,b);
    // activation(a,act);
    // rotate(a);
    // store(a,b);
    return 0;
}
