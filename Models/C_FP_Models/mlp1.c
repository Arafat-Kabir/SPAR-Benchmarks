#include "mlp1.h"
#include "fnlib.h"
#include <stdio.h> 
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define inputSize 10//784
#define m1 10
#define m2 10
#define output 10 //output


static fixp input[inputSize];
static fixp w1[m1][inputSize];//[rows][cols]
static fixp bias1[m1];

static fixp w2[m2][m1];
static fixp bias2[m2];

static fixp w3[output][m2];
static fixp bias3[output];

void initialize(void *param) {
    time_t t;
    srand(5);

    // initialize parameters
    for(int i=0; i<inputSize; i++)
    {
        input[i] = 1<<15;//(fixp)rand()/(fixp)(RAND_MAX/10.0);
    }
    for(int i=0; i<m1; i++)
    {
        for(int j=0; j<inputSize; j++)
        {
            w1[i][j] = 1<<15;//(fixp)rand()/(fixp)(RAND_MAX/10.0);//(fixp)i;
            // printf("%.2f, ", w1[i][j]);
        }
        // printf("\n");
    }
    for(int i=0; i<m1; i++)
    {
        bias1[i]=1<<15;//;
    }

    for(int i=0; i<m2; i++)
    {
        for(int j=0; j<m1; j++)
        {
            w2[i][j] = 1<<15;//(fixp)rand()/(fixp)(RAND_MAX/10.0);//(fixp)i*(fixp)j;
        }
    }
    for(int i=0; i<m2; i++)
    {
        bias2[i]=1<<15;//(fixp)rand()/(fixp)(RAND_MAX/10.0);//50.0f;
    }

    for(int i=0; i<output; i++)
    {
        bias3[i] = 1<<15;//(fixp)rand()/(fixp)(RAND_MAX/10.0);
        for(int j=0; j<m2; j++)
        {
            w3[i][j] = 1<<15;//(fixp)rand()/(fixp)(RAND_MAX/10.0);//(fixp)i*(fixp)j;
        }
    }
}

void write_data_to_file(char *filename) {
    int count = 0;
    FILE *out_file = fopen(filename, "w");
    fprintf(out_file, "{\n");
    //input
    fprintf(out_file, "\"input_size\": %d,\n", inputSize);
    fprintf(out_file, "\"input\": [");
    for(int i=0; i<inputSize; i++)
    {
        if(i>0){fprintf(out_file, ", ");}
        if(i%20 == 19){fprintf(out_file, "\n");}
        fprintf(out_file, "%d", input[i]);
    }
    fprintf(out_file, "],\n");

    //weight1
    fprintf(out_file, "\"weight1_col\": %d,\n", inputSize);
    fprintf(out_file, "\"weight1_row\": %d,\n", m1);
    fprintf(out_file, "\"weight1\": [");
    for(int i=0; i<m1; i++)
    {
        for(int j=0; j<inputSize; j++)
        {
            
            if(count%20 == 19){fprintf(out_file, "\n");}
            fprintf(out_file, "%d", w1[i][j]);
            if((j<inputSize-1)||(i<m1-1)){fprintf(out_file, ", ");}
            count++;
        }
    }
    fprintf(out_file, "],\n");
    //bias1
    fprintf(out_file, "\"bias1_size\": %d,\n", m1);
    fprintf(out_file, "\"bias1\": [");
    for(int i=0; i<m1; i++)
    {
        if(i>0){fprintf(out_file, ", ");}
        if(i%20 == 19){fprintf(out_file, "\n");}
        fprintf(out_file, "%d", bias1[i]);
    }
    fprintf(out_file, "],\n");

    //weight2
    count = 0;
    fprintf(out_file, "\"weight2_col\": %d,\n", m1);
    fprintf(out_file, "\"weight2_row\": %d,\n", m2);
    fprintf(out_file, "\"weight2\": [");
   for(int i=0; i<m2; i++)
    {
        for(int j=0; j<m1; j++)
        {
            
            if(count%20 == 19){fprintf(out_file, "\n");}
            fprintf(out_file, "%d", w2[i][j]);
            if((j<m1-1)||(i<m2-1)){fprintf(out_file, ", ");}
            count++;
        }
    }
    fprintf(out_file, "],\n");
    //bias2
    fprintf(out_file, "\"bias2_size\": %d,\n", m2);
    fprintf(out_file, "\"bias2\": [");
    for(int i=0; i<m2; i++)
    {
        if(i>0){fprintf(out_file, ", ");}
        if(i%20 == 19){fprintf(out_file, "\n");}
        fprintf(out_file, "%d", bias2[i]);
    }
    fprintf(out_file, "],\n");

     //weight3
    count = 0;
    fprintf(out_file, "\"weight3_col\": %d,\n", m2);
    fprintf(out_file, "\"weight3_row\": %d,\n", output);
    fprintf(out_file, "\"weight3\": [");
    for(int i=0; i<output; i++)
    {
        for(int j=0; j<m2; j++)
        {
            
            if(count%20 == 19){fprintf(out_file, "\n");}
            fprintf(out_file, "%d", w3[i][j]);
            if((j<m2-1)||(i<output-1)){fprintf(out_file, ", ");}
            count++;
        }
    }
    fprintf(out_file, "],\n");
    //bias3
    fprintf(out_file, "\"bias3_size\": %d,\n", output);
    fprintf(out_file, "\"bias3\": [");
    for(int i=0; i<output; i++)
    {
        if(i>0){fprintf(out_file, ", ");}
        if(i%20 == 19){fprintf(out_file, "\n");}
        fprintf(out_file, "%d", bias3[i]);
    }
    fprintf(out_file, "]\n");

    fprintf(out_file, "}");
}

int run_inference() { //change this
    initialize(NULL);
    write_data_to_file("../WeightsAndBiases/mlp1.json");
    //first layer
    load_v(1, input, inputSize);
    load_m(2, w1, m1, inputSize);
    e_mul_mv(2, 1, m1, inputSize, 3);
    acc_col(3, m1, inputSize, 0, 4); //accumulated vector in reg4
    load_v_t(3, bias1, m1); //load bias in turned orientation to// change to another orientation for loading
    add(3, 4, 1); //add bias to vector
    printreg_segment(3, 10, 10);
    ReLU(1, 1);
    rotate(1);//rotate to correct the vector. May not be neccessary later

    // //second layer
    // load_m(2, w2, m2, m1);
    // e_mul_mv(2, 1, m2, m1, 3);
    // acc_col(3, m2, m1, 0, 4);
    // load_v_t(3, bias2, m2);
    // add(3, 4, 1);
    // ReLU(1, 1);
    // rotate(1);

    // //output
    // load_m(2, w3, output, m2);
    // e_mul_mv(2, 1, output, m2, 3);
    // acc_col(3, output, m2, 0, 4);
    // load_v_t(3, bias2, m2);
    // add(3, 4, 1);
    // rotate(1); //rotate for now. The softmax will have to be implemented in software later.

    printreg_to_file(1, 1, output, "../outputs/mlp_c_output.txt");
    //store here
    return 0;
}
