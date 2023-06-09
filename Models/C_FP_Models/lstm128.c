#include <stdio.h>
#include <stdlib.h>
#include "fnlib.h"
#include "fplib.h"
#define x_size 65
#define H0_size 128
#define C0_size 128
#define W1_Rows 128
#define W2_Rows 128
#define WFC_Rows 65

fixp X0[x_size];
fixp H0[H_size];
fixp C0[C_size];

double X0d[x_size];
double H0d[H_size];
double C0d[C_size];

//----layer 1 weights and biases----//
//weights and biases for i1 (i of layer 1)
fixp Wii1[W1_Rows][x_size];
fixp Bii1[W1_Rows];
fixp Whi1[W1_Rows][H_size];
fixp Bhi1[W1_Rows];
//f1
fixp Wif1[W1_Rows][x_size];
fixp Bif1[W1_Rows];
fixp Whf1[W1_Rows][H_size];
fixp Bhf1[W1_Rows];
//g1
fixp Wig1[W1_Rows][x_size];
fixp Big1[W1_Rows];
fixp Whg1[W1_Rows][H_size];
fixp Bhg1[W1_Rows];
//o1
fixp Wio1[W1_Rows][x_size];
fixp Bio1[W1_Rows];
fixp Who1[W1_Rows][H_size];
fixp Bho1[W1_Rows];

//conversion variable (turns Ht-1 doubleo predicted x for next input)
fixp Bernoulli1[x_size][W1_Rows];

//----layer 2 weights and biases----//
//i2
fixp Wii2[W2_Rows][x_size];
fixp Bii2[W2_Rows];
fixp Whi2[W2_Rows][H_size];
fixp Bhi2[W2_Rows];
//f2
fixp Wif2[W2_Rows][x_size];
fixp Bif2[W2_Rows];
fixp Whf2[W2_Rows][H_size];
fixp Bhf2[W2_Rows];
//g2
fixp Wig2[W2_Rows][x_size];
fixp Big2[W2_Rows];
fixp Whg2[W2_Rows][H_size];
fixp Bhg2[W2_Rows];
//o2
fixp Wio2[W2_Rows][x_size];
fixp Bio2[W2_Rows];
fixp Who2[W2_Rows][H_size];
fixp Bho2[W2_Rows];

//----Fully Connected Layer----//
fixp WFC[WFC_Rows][W2_Rows];
fixp BFC[WFC_Rows];

//----layer 1 weights and biases----//
//weights and biases for i1 (i of layer 1)
double Wii1d[W1_Rows][x_size];
double Bii1d[W1_Rows];
double Whi1d[W1_Rows][H_size];
double Bhi1d[W1_Rows];
//f1
double Wif1d[W1_Rows][x_size];
double Bif1d[W1_Rows];
double Whf1d[W1_Rows][H_size];
double Bhf1d[W1_Rows];
//g1
double Wig1d[W1_Rows][x_size];
double Big1d[W1_Rows];
double Whg1d[W1_Rows][H_size];
double Bhg1d[W1_Rows];
//o1
double Wio1d[W1_Rows][x_size];
double Bio1d[W1_Rows];
double Who1d[W1_Rows][H_size];
double Bho1d[W1_Rows];

//conversion variable (turns Ht-1 doubleo predicted x for next input)
double Bernoulli1d[x_size][W1_Rows];

//----layer 2 weights and biases----//
//i2
double Wii2d[W2_Rows][x_size];
double Bii2d[W2_Rows];
double Whi2d[W2_Rows][H_size];
double Bhi2d[W2_Rows];
//f2
double Wif2d[W2_Rows][x_size];
double Bif2d[W2_Rows];
double Whf2d[W2_Rows][H_size];
double Bhf2d[W2_Rows];
//g2
double Wig2d[W2_Rows][x_size];
double Big2d[W2_Rows];
double Whg2d[W2_Rows][H_size];
double Bhg2d[W2_Rows];
//o2
double Wio2d[W2_Rows][x_size];
double Bio2d[W2_Rows];
double Who2d[W2_Rows][H_size];
double Bho2d[W2_Rows];

//----Fully Connected Layer----//
double WFCd[WFC_Rows][W2_Rows];
double BFCd[WFC_Rows];

void initialize_lstm128(void *param){
    srand(24537890);
    double highestFloat = 10.0f;
    for(int i=0; i<x_size; i++){
        X0d[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
    }
    for(int i=0; i<H_size; i++){
        H0d[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
    }
    for(int i=0; i<C_size; i++){
        C0d[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
    }
    for(int i=0; i<W1_Rows; i++)
    {
        Bii1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bif1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Big1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bio1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<x_size; j++)
        {
            Wii1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wif1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wig1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wio1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    for(int i=0; i<W1_Rows; i++)
    {
        Bhi1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhf1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhg1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bho1d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<H_size; j++)
        {
            Whi1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whf1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whg1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Who1d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    //dropout
    for(int i=0; i<x_size; i++)
    {
        for(int j=0; j<W1_Rows; j++)
        {
            int r = rand()%2;
            if(r==1){Bernoulli1d[i][j] = 1.0f;}
            else {Bernoulli1d[i][j] = 0.0f;}
            
        }
    }

    for(int i=0; i<W2_Rows; i++)
    {
        Bii2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bif2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Big2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bio2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<x_size; j++)
        {
            Wii2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wif2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wig2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wio2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    for(int i=0; i<W2_Rows; i++)
    {
        Bhi2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhf2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhg2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bho2d[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<W1_Rows; j++)
        {
            Whi2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whf2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whg2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Who2d[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    for(int i=0; i<WFC_Rows; i++)
    {
        BFCd[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<W2_Rows; j++)
        {
            WFCd[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    
    fixp X0[x_size];
    fixp H0[H_size];
    fixp C0[C_size];
    float_to_fixed_array(&X0d, x_size, &X0);
    float_to_fixed_array(&H0d, H_size, &H0);
    float_to_fixed_array(&C0d, C_size, &C0);

    float_to_fixed_array(&Wii1d, W1_Rows*x_size, &Wii1);
    float_to_fixed_array(&Bii1d, W1_Rows, &Bii1);
    float_to_fixed_array(&Whi1d, W1_Rows*H_size, &Whi1);
    float_to_fixed_array(&Bhi1d, W1_Rows, &Bhi1);
    float_to_fixed_array(&Wif1d, W1_Rows*x_size, &Wif1);
    float_to_fixed_array(&Bif1d, W1_Rows, &Bif1);
    float_to_fixed_array(&Whf1d, W1_Rows*H_size, &Whf1);
    float_to_fixed_array(&Bhf1d, W1_Rows, &Bhf1);
    float_to_fixed_array(&Wig1d, W1_Rows*x_size, &Wig1);
    float_to_fixed_array(&Big1d, W1_Rows, &Big1);
    float_to_fixed_array(&Whg1d, W1_Rows*H_size, &Whg1);
    float_to_fixed_array(&Bhg1d, W1_Rows, &Bhg1);
    float_to_fixed_array(&Wio1d, W1_Rows*x_size, &Wio1);
    float_to_fixed_array(&Bio1d, W1_Rows, &Bio1);
    float_to_fixed_array(&Who1d, W1_Rows*H_size, &Who1);
    float_to_fixed_array(&Bho1d, W1_Rows, &Bho1);

    float_to_fixed_array(&Wii2d, W2_Rows*x_size, &Wii2);
    float_to_fixed_array(&Bii2d, W2_Rows, &Bii2);
    float_to_fixed_array(&Whi2d, W2_Rows*W1_Rows, &Whi2);
    float_to_fixed_array(&Bhi2d, W2_Rows, &Bhi2);
    float_to_fixed_array(&Wif2d, W2_Rows*x_size, &Wif2);
    float_to_fixed_array(&Bif2d, W2_Rows, &Bif2);
    float_to_fixed_array(&Whf2d, W2_Rows*W1_Rows, &Whf2);
    float_to_fixed_array(&Bhf2d, W2_Rows, &Bhf2);
    float_to_fixed_array(&Wig2d, W2_Rows*x_size, &Wig2);
    float_to_fixed_array(&Big2d, W2_Rows, &Big2);
    float_to_fixed_array(&Whg2d, W2_Rows*W1_Rows, &Whg2);
    float_to_fixed_array(&Bhg2d, W2_Rows, &Bhg2);
    float_to_fixed_array(&Wio2d, W2_Rows*x_size, &Wio2);
    float_to_fixed_array(&Bio2d, W2_Rows, &Bio2);
    float_to_fixed_array(&Who2d, W2_Rows*W1_Rows, &Who2);
    float_to_fixed_array(&Bho2d, W2_Rows, &Bho2);

    float_to_fixed_array(&WFCd, WFC_Rows*W2_Rows, WFC);
    float_to_fixed_array(&BFCd, WFC_Rows, BFC);
    return;
}

void populate_lstm128_1s(){
    for(int i=0; i<x_size; i++){
        X0[i] = 1.0f;
    }
    for(int i=0; i<H_size; i++){
        H0[i] = 1.0f;
    }
    for(int i=0; i<C_size; i++){
        C0[i] = 1.0f;
    }
    for(int i=0; i<W1_Rows; i++)
    {
        Bii1[i]=1.0f;
        Bif1[i]=1.0f;
        Big1[i]=1.0f;
        Bio1[i]=1.0f;
        for(int j=0; j<x_size; j++)
        {
            Wii1[i][j]=1.0f;
            Wif1[i][j]=1.0f;
            Wig1[i][j]=1.0f;
            Wio1[i][j]=1.0f;
        }
    }
    for(int i=0; i<W1_Rows; i++)
    {
        Bhi1[i]=1.0f;
        Bhf1[i]=1.0f;
        Bhg1[i]=1.0f;
        Bho1[i]=1.0f;
        for(int j=0; j<H_size; j++)
        {
            Whi1[i][j]=1.0f;
            Whf1[i][j]=1.0f;
            Whg1[i][j]=1.0f;
            Who1[i][j]=1.0f;
        }
    }
    //dropout
    for(int i=0; i<x_size; i++)
    {
        for(int j=0; j<W1_Rows; j++)
        {
            Bernoulli1[i][j] = 1.0f;
        }
    }

    for(int i=0; i<W2_Rows; i++)
    {
        Bii2[i]=1.0f;
        Bif2[i]=1.0f;
        Big2[i]=1.0f;
        Bio2[i]=1.0f;
        for(int j=0; j<x_size; j++)
        {
            Wii2[i][j]=1.0f;
            Wif2[i][j]=1.0f;
            Wig2[i][j]=1.0f;
            Wio2[i][j]=1.0f;
        }
    }
    for(int i=0; i<W2_Rows; i++)
    {
        Bhi2[i]=1.0f;
        Bhf2[i]=1.0f;
        Bhg2[i]=1.0f;
        Bho2[i]=1.0f;
        for(int j=0; j<W1_Rows; j++)
        {
            Whi2[i][j]=1.0f;
            Whf2[i][j]=1.0f;
            Whg2[i][j]=1.0f;
            Who2[i][j]=1.0f;
        }
    }
    for(int i=0; i<WFC_Rows; i++)
    {
        BFC[i]=1.0f;
        for(int j=0; j<W2_Rows; j++)
        {
            WFC[i][j]=1.0f;
        }
    }
}

void lstm128_to_json(char *filename){
    FILE *outfile = fopen(filename, "w");
    fprintf(outfile, "{\n");
    fprintf(outfile, "\"X_Size\": %d,\n", x_size);
    fprintf(outfile, "\"H_Size\": %d,\n", H_size);
    fprintf(outfile, "\"C_Size\": %d,\n", C_size);
    fprintf(outfile, "\"W1_Rows\": %d,\n", W1_Rows);
    fprintf(outfile, "\"W2_Rows\": %d,\n", W2_Rows);
    fprintf(outfile, "\"Output_Size\": %d,\n", WFC_Rows);
    write_array_to_file(outfile, X0, x_size, "Input", 0);
    write_array_to_file(outfile, H0, H_size, "H0", 0);
    write_array_to_file(outfile, C0, C_size, "C0", 0);
    //weights and biases
    write_array_to_file(outfile, Wii1, x_size*W1_Rows, "Wii1", 0);
    write_array_to_file(outfile, Bii1, W1_Rows, "Bii1", 0);
    write_array_to_file(outfile, Whi1, H_size*W1_Rows, "Whi1", 0);
    write_array_to_file(outfile, Bhi1, W1_Rows, "Bhi1", 0);
    write_array_to_file(outfile, Wif1, x_size*W1_Rows, "Wif1", 0);
    write_array_to_file(outfile, Bif1, W1_Rows, "Bif1", 0);
    write_array_to_file(outfile, Whf1, H_size*W1_Rows, "Whf1", 0);
    write_array_to_file(outfile, Bhf1, W1_Rows, "Bhf1", 0);
    write_array_to_file(outfile, Wig1, x_size*W1_Rows, "Wig1", 0);
    write_array_to_file(outfile, Big1, W1_Rows, "Big1", 0);
    write_array_to_file(outfile, Whg1, H_size*W1_Rows, "Whg1", 0);
    write_array_to_file(outfile, Bhg1, W1_Rows, "Bhg1", 0);
    write_array_to_file(outfile, Wio1, x_size*W1_Rows, "Wio1", 0);
    write_array_to_file(outfile, Bio1, W1_Rows, "Bio1", 0);
    write_array_to_file(outfile, Who1, H_size*W1_Rows, "Who1", 0);
    write_array_to_file(outfile, Bho1, W1_Rows, "Bho1", 0);

    write_array_to_file(outfile, Bernoulli1, W1_Rows*x_size, "Dropout1", 0);

    write_array_to_file(outfile, Wii2, x_size*W2_Rows, "Wii2", 0);
    write_array_to_file(outfile, Bii2, W2_Rows, "Bii2", 0);
    write_array_to_file(outfile, Whi2, W1_Rows*W2_Rows, "Whi2", 0);
    write_array_to_file(outfile, Bhi2, W2_Rows, "Bhi2", 0);
    write_array_to_file(outfile, Wif2, x_size*W2_Rows, "Wif2", 0);
    write_array_to_file(outfile, Bif2, W2_Rows, "Bif2", 0);
    write_array_to_file(outfile, Whf2, W1_Rows*W2_Rows, "Whf2", 0);
    write_array_to_file(outfile, Bhf2, W2_Rows, "Bhf2", 0);
    write_array_to_file(outfile, Wig2, x_size*W2_Rows, "Wig2", 0);
    write_array_to_file(outfile, Big2, W2_Rows, "Big2", 0);
    write_array_to_file(outfile, Whg2, W1_Rows*W2_Rows, "Whg2", 0);
    write_array_to_file(outfile, Bhg2, W2_Rows, "Bhg2", 0);
    write_array_to_file(outfile, Wio2, x_size*W2_Rows, "Wio2", 0);
    write_array_to_file(outfile, Bio2, W2_Rows, "Bio2", 0);
    write_array_to_file(outfile, Who2, W1_Rows*W2_Rows, "Who2", 0);
    write_array_to_file(outfile, Bho2, W2_Rows, "Bho2", 0);
    write_array_to_file(outfile, WFC, WFC_Rows*W2_Rows, "WFC", 0);
    write_array_to_file(outfile, BFC, WFC_Rows, "BFC", 1);

    fprintf(outfile, "}");
    return;
}

int run_inference_lstm128(){
    initialize_lstm128(NULL);
    lstm128_to_json("../WeightsAndBiases/lstm128_fp.json");
    //void lstm_gate(x,Wi,Bi,h,Wh,Bh,x_size,h_size,output_size,act_func,temp1,temp2,temp3,des)
    
    //-----------------------Layer 1--------------------------//
    //x in 1
    //Wii, Bii in 2, 3 respectivley
    //h in 4
    //Whi, Bhi in 5, 6 respectively
    load_v(1, X0, x_size);
    load_m(2, Wii1, W1_Rows, x_size);
    load_v_t(3, Bii1, W1_Rows);
    load_v(4, H0, H_size);
    load_m(5, Whi1, W1_Rows, H_size);
    load_v_t(6, Bif1, W1_Rows);
    //i in register 7. 29, 30, and 31 are for temporary registers
    lstm_gate(1,2,3,4,5,6, x_size, H_size, W1_Rows, 1, 29, 30, 31, 7); //i1 in 7
    printreg_segment(7, 10, 10);
    //calculate f1
    load_m(2, Wif1, W1_Rows, x_size);
    load_v_t(3, Bif1, W1_Rows);
    load_m(5, Whf1, W1_Rows, H_size);
    load_v_t(6, Bhf1, W1_Rows);
    lstm_gate(1,2,3,4,5,6, x_size, H_size, W1_Rows, 1, 29, 30, 31, 8); //f1 in reg 8

    //calculate g
    load_m(2, Wig1, W1_Rows, x_size);
    load_v_t(3, Big1, W1_Rows);
    load_m(5, Whg1, W1_Rows, H_size);
    load_v_t(6, Bhg1, W1_Rows);                  // vvv for tanh
    lstm_gate(1,2,3,4,5,6, x_size, H_size, W1_Rows, 2, 29, 30, 31, 9); //g1 in reg 9
    //calculate o
    load_m(2, Wio1, W1_Rows, x_size);
    load_v_t(3, Bio1, W1_Rows);
    load_m(5, Who1, W1_Rows, H_size);
    load_v_t(6, Bho1, W1_Rows);
    lstm_gate(1,2,3,4,5,6, x_size, H_size, W1_Rows, 1, 29, 30, 31, 10); //o1 in reg 10
    //calculate C1
    load_v(12, C0, C_size); //C0 in reg 12
    e_mul(8, 12, 13); //f1*c0 in reg 13
    e_mul(7, 9, 14);//i1*g1 in reg 14 
    add(13, 14, 12); //overwrite with C1 in 12

    //calculate H1
    rotate_mov(12, 13); //setup C1 copy for activation
    activation(13, 14, 2); //apply tanh to c1
    e_mul(10, 14, 4); //H1 = tanh(c1) * o1

    //predict X1
    load_m(13, Bernoulli1, x_size, W1_Rows);
    e_mul_mv(13, 4, x_size, W1_Rows, 14); //h1*bernoulli
    acc_col(14, x_size, W1_Rows, 0, 1); //overwrite x0 with pred-x1
    rotate(1);
    // printreg_segment(1, 1, 65);

    //-----------Layer 2----------//
    //x1 in 1, h1 in 4, c1 in 12
    load_m(2, Wii2, W1_Rows, x_size);
    load_v_t(3, Bii2, W1_Rows);
    load_m(5, Whi2, W1_Rows, W1_Rows);
    load_v_t(6, Bif2, W1_Rows);
    //calculate i
    lstm_gate(1,2,3,4,5,6, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 7); //i2 in 7
    // printreg_segment(7, 1, 128);

    //calculate f2
    load_m(2, Wif2, W2_Rows, x_size);
    load_v_t(3, Bif2, W2_Rows);
    load_m(5, Whf2, W2_Rows, W1_Rows);
    load_v_t(6, Bhf2, W2_Rows);
    lstm_gate(1,2,3,4,5,6, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 8); //f2 in reg 8
    // printreg_segment(8, 1, 128);
    //calculate g2
    load_m(2, Wig2, W2_Rows, x_size);
    load_v_t(3, Big2, W2_Rows);
    load_m(5, Whg2, W2_Rows, W1_Rows);
    load_v_t(6, Bhg2, W2_Rows);                  // vvv for tanh
    lstm_gate(1,2,3,4,5,6, x_size, W1_Rows, W2_Rows, 2, 29, 30, 31, 9); //g2 in reg 9
    // printreg_segment(9, 1, 128);

    //calculate o2
    load_m(2, Wio2, W2_Rows, x_size);
    load_v_t(3, Bio2, W2_Rows);
    load_m(5, Who2, W2_Rows, W1_Rows);
    load_v_t(6, Bho2, W2_Rows);
    lstm_gate(1,2,3,4,5,6, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 10); //o2 in reg 10
    // printreg_segment(10, 1, 128);

    //calculate C2
    e_mul(8, 12, 13); //f2*c1 in reg 13
    e_mul(7, 9, 14);//i2*g2 in reg 14 
    add(13, 14, 12); //overwrite with C2 in 12
    // printreg_segment(12, 1, 128);

    //calculate H2
    rotate_mov(12, 13); //setup C2 copy for activation
    activation(13, 14, 2); //apply tanh to c2
    e_mul(10, 14, 16); //H2 = tanh(c2) * o2. Don't overwrite H1, might be needed for FC layer
    // printreg_segment(16, 1, 128);

    //fully connected layer using just h2 for now
    load_m(1, WFC, WFC_Rows, W2_Rows);
    // printreg_segment(1, 65, 128);
    load_v_t(2, BFC, WFC_Rows); //todo: bias not matching json
    e_mul_mv(1, 16, WFC_Rows, W2_Rows, 3);
    acc_col(3, WFC_Rows, W2_Rows, 0, 4);
    // printreg_segment(4, 1, 65);

    add(4, 2, 5);
    rotate(5);
    // printreg_segment(5, 1, 65);
    printreg_to_file(5, 1, 65, "../outputs/lstm128_c_output.txt");
    return 0;
}


//todo: 
//* randomly generate values and output them to json
//* add a way to load those values to the python model
//* make a run file and compare the outputs

