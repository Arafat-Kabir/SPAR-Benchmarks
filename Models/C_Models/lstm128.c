#include <stdio.h>
#include <stdlib.h>
#include "fnlib.h"
#define x_size 65
#define H0_size 128
#define C0_size 128
#define W1_Rows 128
#define W2_Rows 128
#define WFC_Rows 65

double X0[x_size];
double H0[H0_size];
double C0[C0_size];
//H1, C1, etc are calculated at runtime
//Wi is for x, Wh is for H
//Wii is for finding i, Wif is for finding f, etc

//----layer 1 weights and biases----//
//weights and biases for i1 (i of layer 1)
double Wii1[W1_Rows][x_size];
double Bii1[W1_Rows];
double Whi1[W1_Rows][H0_size];
double Bhi1[W1_Rows];
//f1
double Wif1[W1_Rows][x_size];
double Bif1[W1_Rows];
double Whf1[W1_Rows][H0_size];
double Bhf1[W1_Rows];
//g1
double Wig1[W1_Rows][x_size];
double Big1[W1_Rows];
double Whg1[W1_Rows][H0_size];
double Bhg1[W1_Rows];
//o1
double Wio1[W1_Rows][x_size];
double Bio1[W1_Rows];
double Who1[W1_Rows][H0_size];
double Bho1[W1_Rows];

//conversion variable (turns Ht-1 doubleo predicted x for next input)
double Bernoulli1[x_size][W1_Rows];

//----layer 2 weights and biases----//
//i2
double Wii2[W2_Rows][x_size];
double Bii2[W2_Rows];
double Whi2[W2_Rows][H0_size];
double Bhi2[W2_Rows];
//f2
double Wif2[W2_Rows][x_size];
double Bif2[W2_Rows];
double Whf2[W2_Rows][H0_size];
double Bhf2[W2_Rows];
//g2
double Wig2[W2_Rows][x_size];
double Big2[W2_Rows];
double Whg2[W2_Rows][H0_size];
double Bhg2[W2_Rows];
//o2
double Wio2[W2_Rows][x_size];
double Bio2[W2_Rows];
double Who2[W2_Rows][H0_size];
double Bho2[W2_Rows];

//----Fully Connected Layer----//
double WFC[WFC_Rows][W2_Rows];
double BFC[WFC_Rows];

void initialize_lstm128(void *param){
    return;
}

void populate_lstm128_1s(){
    for(int i=0; i<x_size; i++){
        X0[i] = 1.0f;
    }
    for(int i=0; i<H0_size; i++){
        H0[i] = 1.0f;
    }
    for(int i=0; i<C0_size; i++){
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
        for(int j=0; j<H0_size; j++)
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

int run_inference_lstm128(){
    populate_lstm128_1s();
    //void lstm_gate(x,Wi,Bi,h,Wh,Bh,x_size,h_size,output_size,act_func,temp1,temp2,temp3,des)
    
    //-----------------------Layer 1--------------------------//
    //x in 1
    //Wii, Bii in 2, 3 respectivley
    //h in 4
    //Whi, Bhi in 5, 6 respectively
    load_v(1, X0, x_size);
    load_m(2, Wii1, W1_Rows, x_size);
    load_v_t(3, Bii1, W1_Rows);
    load_v(4, H0, H0_size);
    load_m(5, Whi1, W1_Rows, H0_size);
    load_v_t(6, Bif1, W1_Rows);
    //i in register 7. 29, 30, and 31 are for temporary registers
    lstm_gate(1,2,3,4,5,6, x_size, H0_size, W1_Rows, 1, 29, 30, 31, 7); //i1 in 7

    //calculate f1
    load_m(2, Wif1, W1_Rows, x_size);
    load_v_t(3, Bif1, W1_Rows);
    load_m(5, Whf1, W1_Rows, H0_size);
    load_v_t(6, Bhf1, W1_Rows);
    lstm_gate(1,2,3,4,5,6, x_size, H0_size, W1_Rows, 1, 29, 30, 31, 8); //f1 in reg 8
    // printreg_v(8);

    //calculate g
    load_m(2, Wig1, W1_Rows, x_size);
    load_v_t(3, Big1, W1_Rows);
    load_m(5, Whg1, W1_Rows, H0_size);
    load_v_t(6, Bhg1, W1_Rows);                  // vvv for tanh
    lstm_gate(1,2,3,4,5,6, x_size, H0_size, W1_Rows, 2, 29, 30, 31, 9); //g1 in reg 9
    //calculate o
    load_m(2, Wio1, W1_Rows, x_size);
    load_v_t(3, Bio1, W1_Rows);
    load_m(5, Who1, W1_Rows, H0_size);
    load_v_t(6, Bho1, W1_Rows);
    lstm_gate(1,2,3,4,5,6, x_size, H0_size, W1_Rows, 1, 29, 30, 31, 10); //o1 in reg 10
    //calculate C1
    load_v(12, C0, C0_size); //C0 in reg 12
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

    //-----------Layer 2----------//
    //x1 in 1, h1 in 4, c1 in 12
    load_m(2, Wii2, W1_Rows, x_size);
    load_v_t(3, Bii2, W1_Rows);
    load_m(5, Whi2, W1_Rows, W1_Rows);
    load_v_t(6, Bif2, W1_Rows);
    //calculate i
    lstm_gate(1,2,3,4,5,6, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 7); //i2 in 7
    // printreg_segment(7, 1, 128);

    //calculate f1
    load_m(2, Wif2, W2_Rows, x_size);
    load_v_t(3, Bif2, W2_Rows);
    load_m(5, Whf2, W2_Rows, W1_Rows);
    load_v_t(6, Bhf2, W2_Rows);
    lstm_gate(1,2,3,4,5,6, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 8); //f2 in reg 8
    // printreg_segment(8, 1, 128);
    //calculate g
    load_m(2, Wig2, W2_Rows, x_size);
    load_v_t(3, Big2, W2_Rows);
    load_m(5, Whg2, W2_Rows, W1_Rows);
    load_v_t(6, Bhg2, W2_Rows);                  // vvv for tanh
    lstm_gate(1,2,3,4,5,6, x_size, W1_Rows, W2_Rows, 2, 29, 30, 31, 9); //g2 in reg 9
    // printreg_segment(9, 1, 128);

    //calculate o
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
    load_v_t(2, BFC, WFC_Rows);
    e_mul_mv(1, 16, WFC_Rows, W2_Rows, 2);
    acc_col(2, WFC_Rows, W2_Rows, 0, 3);
    add(3, 2, 5);
    rotate(5);
    printreg_segment(5, 1, 128);

    return 0;
}

//todo: 
//* randomly generate values and output them to json
//* add a way to load those values to the python model
//* make a run file and compare the outputs

