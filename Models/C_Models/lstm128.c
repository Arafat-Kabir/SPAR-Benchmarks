#include <stdio.h>
#include <stdlib.h>

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
double Wii1[x_size][W1_Rows];
double Bii[W1_Rows];
double Whi1[H0_size][W1_Rows];
double Bhi[W1_Rows];
//f1
double Wif1[x_size][W1_Rows];
double Bif1[W1_Rows];
double Whi1[H0_size][W1_Rows];
double Bhf1[W1_Rows];
//g1
double Wig1[x_size][W1_Rows];
double Big1[W1_Rows];
double Whg1[H0_size][W1_Rows];
double Bhg1[W1_Rows];
//o1
double Wio1[x_size][W1_Rows];
double Bio1[W1_Rows];
double Who1[H0_size][W1_Rows];
double Bho1[W1_Rows];

//conversion variable (turns Ht-1 doubleo predicted x for next input)
double Bernoulli1[W1_Rows][x_size];

//----layer 2 weights and biases----//
//i2
double Wii2[x_size][W2_Rows];
double Bii[W2_Rows];
double Whi2[H0_size][W2_Rows];
double Bhi[W2_Rows];
//f2
double Wif2[x_size][W2_Rows];
double Bif2[W2_Rows];
double Whi2[H0_size][W2_Rows];
double Bhf2[W2_Rows];
//g2
double Wig2[x_size][W2_Rows];
double Big2[W2_Rows];
double Whg2[H0_size][W2_Rows];
double Bhg2[W2_Rows];
//o2
double Wio2[x_size][W2_Rows];
double Bio2[W2_Rows];
double Who2[H0_size][W2_Rows];
double Bho2[W2_Rows];

//----Fully Connected Layer----//
double WFC[W2_Rows][WFC_Rows];
double BFC[WFC_Rows];

void initialize_lstm128(void *param){
    return;
}

int run_inference_lstm128(){
    
    return 0;
}