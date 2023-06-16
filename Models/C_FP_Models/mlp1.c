#include "mlp1.h"
#include "fnlib.h"
#include <stdio.h> 
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define inputSize 16//784
#define m1 10
#define m2 10
#define output 8 //output


static fixp input[inputSize];
static fixp w1[m1][inputSize];//[rows][cols]
static fixp bias1[m1];

static fixp w2[m2][m1];
static fixp bias2[m2];

static fixp w3[output][m2];
static fixp bias3[output];

static double inputd[inputSize];
static double w1d[m1][inputSize];//[rows][cols]
static double bias1d[m1];

static double w2d[m2][m1];
static double bias2d[m2];

static double w3d[output][m2];
static double bias3d[output];

void initialize(void *param) {
    time_t t;
    srand(3);

    for(int i=0; i<inputSize; i++)
    {
        inputd[i] = (double)rand()/(double)(RAND_MAX/1.0);
    }
    for(int i=0; i<m1; i++)
    {
        for(int j=0; j<inputSize; j++)
        {
            w1d[i][j] = (double)rand()/(double)(RAND_MAX/1.0);//(double)i;
            // printf("%.2f, ", w1[i][j]);
        }
        // printf("\n");
    }
    for(int i=0; i<m1; i++)
    {
        bias1d[i]=-190.0f;
    }

    for(int i=0; i<m2; i++)
    {
        for(int j=0; j<m1; j++)
        {
            w2d[i][j] = (double)rand()/(double)(RAND_MAX/1.0);//(double)i*(double)j;
        }
    }
    for(int i=0; i<m2; i++)
    {
        bias2d[i]=(double)rand()/(double)(RAND_MAX/1.0)-250.0;//50.0f;
    }

    for(int i=0; i<output; i++)
    {
        bias3d[i] = (double)rand()/(double)(RAND_MAX/1.0)-850;
        for(int j=0; j<m2; j++)
        {
            w3d[i][j] = (double)rand()/(double)(RAND_MAX/1.0);//(double)i*(double)j;
        }
    }

    // initialize parameters
    float_to_fixed_array(&inputd,inputSize, &input);
    float_to_fixed_array(&w1d, m1*inputSize, &w1);
    float_to_fixed_array(&bias1d, m1, &bias1 );
    float_to_fixed_array(&w2d, m2*m1, &w2);
    float_to_fixed_array(&bias2d, m2, &bias2);
    float_to_fixed_array(&w3d, output*m2, &w3);
    float_to_fixed_array(&bias3d, output, &bias3);
    // for(int i=0; i<inputSize; i++)
    // {
    //     int32_t number = input[i];
    //     int32_t fraction = (100000*(number&(0xFFFF)))/(65536);
    //     if(fraction < 0) {fraction *= -1;}
    //     printf("%d.%0.5d, ", number>>16, fraction);
    //     printf("%d, ", number);
    //     printf("\t %llf\n", inputd[i]);
    // }
}

void write_data_to_file(char *filename) {
    int count = 0;
    FILE *out_file = fopen(filename, "w");
    fprintf(out_file, "{\n");
    //input
    fprintf(out_file, "\"input_size\": %d,\n", inputSize);
    write_array_to_file(out_file, &inputd, inputSize, "input", 0);
    
    //weight1
    fprintf(out_file, "\"weight1_col\": %d,\n", inputSize);
    fprintf(out_file, "\"weight1_row\": %d,\n", m1);
    write_array_to_file(out_file, &w1d, m1*inputSize, "weight1", 0);
    //bias1
    fprintf(out_file, "\"bias1_size\": %d,\n", m1);
    write_array_to_file(out_file, &bias1d, m1, "bias1", 0);
    //weight2 
    fprintf(out_file, "\"weight2_col\": %d,\n", m1);
    fprintf(out_file, "\"weight2_row\": %d,\n", m2);
    write_array_to_file(out_file, &w2d, m2*m1, "weight2", 0);
    //bias2
    fprintf(out_file, "\"bias2_size\": %d,\n", m2);
    write_array_to_file(out_file, &bias2d, m2, "bias2", 0);
    //weight3
    fprintf(out_file, "\"weight3_col\": %d,\n", m2);
    fprintf(out_file, "\"weight3_row\": %d,\n", output);
    write_array_to_file(out_file, &w3d, output*m2, "weight3", 0);
    //bias3
    fprintf(out_file, "\"bias3_size\": %d,\n", output);
    write_array_to_file(out_file, &bias3d, output, "bias3", 1);


    fprintf(out_file, "}");
}

int run_inference() { //change this
    initialize(NULL);
    write_data_to_file("../WeightsAndBiases/mlp1_fp.json");
    //first layer
    load_v(1, input, inputSize);
    load_m(2, w1, m1, inputSize);
    e_mul_mv(2, 1, m1, inputSize, 3);
    acc_col(3, m1, inputSize, 0, 4); //accumulated vector in reg4
    load_v_t(3, bias1, m1); //load bias in turned orientation to// change to another orientation for loading
    add(3, 4, 1); //add bias to vector
    ReLU(1, 1);
    rotate(1);//rotate to correct the vector. May not be neccessary later
    printf("\n");    

    // //second layer
    load_m(2, w2, m2, m1);
    e_mul_mv(2, 1, m2, m1, 3);
    acc_col(3, m2, m1, 0, 4);
    load_v_t(3, bias2, m2);
    add(3, 4, 1);
    ReLU(1, 1);
    rotate(1);

    // //output
    load_m(2, w3, output, m2);
    printreg_segment(2, 5, 5);
    e_mul_mv(2, 1, output, m2, 3);
    acc_col(3, output, m2, 0, 4);
   
    load_v_t(3, bias3, output);
    add(3, 4, 1);
    printreg_segment(3, 10, 1);
    rotate(1); //rotate for now. The softmax will have to be implemented in software later.
    printreg_to_file(1, 1, output, "../outputs/mlp_c_fp_output.txt");
    
    //store here
    return 0;
}
