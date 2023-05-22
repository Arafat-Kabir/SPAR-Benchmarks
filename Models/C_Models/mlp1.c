#include "mlp1.h"
#include "fnlib.h"
#include <stdio.h> 

#define inputSize 784//784
#define m1 100
#define m2 100//output

static double input[inputSize];
static double w1[m1][inputSize];//[rows][cols]
static double bias1[m1];

static double w2[m2][m1];
static double bias2[m2];

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
            w1[i][j] = (double)i;
            // printf("%.2f, ", w1[i][j]);
        }
        // printf("\n");
    }
    for(int i=0; i<m1; i++)
    {
        bias1[i]=-10.0f;
    }

    for(int i=0; i<m2; i++)
    {
        for(int j=0; j<m1; j++)
        {
            w2[i][j] = (double)i*(double)j;
        }
    }
    for(int i=0; i<m2; i++)
    {
        bias2[i]=50.0f;
    }
}

void write_data_to_file(char *file) {
    
}

int run_inference() { //change this
    initialize(NULL);

    //first layer
    load_v(1, input, inputSize);
    load_m(2, w1, m1, inputSize);
    e_mul_mv(2, 1, m1, inputSize, 3);
    acc_col(3, m1, inputSize, 0, 4); //accumulated vector in reg4
    load_v_t(3, bias1, m1); //load bias in turned orientation to// change to another orientation for loading
    add(3, 4, 1); //add bias to vector
    ReLU(1, 1);
    rotate(1);//rotate to correct the vector. May not be neccessary later

    //second layer
    load_m(2, w2, m2, m1);
    e_mul_mv(2, 1, m2, m1, 3);
    acc_col(3, m2, m1, 0, 4);
    load_v_t(3, bias2, m2);
    add(3, 4, 1);
    ReLU(1, 1);
    rotate(1);
    printreg_to_file(1, 1, m2, "../outputs/mlp_c_output.txt");
    //store here
    return 0;
}
