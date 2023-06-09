#include <stdio.h>
#include <stdlib.h>
#include "fnlib.h"
#define x_size 65
#define H_size 128
#define C_size 128
#define W1_Rows 128
#define W2_Rows 128
#define WFC_Rows 65

double X[x_size];
double H_l1[H_size];
double C_l1[C_size];
double H_l2[H_size];
double C_l2[C_size];
//H1, C1, etc are calculated at runtime
//Wi is for x, Wh is for H
//Wii is for finding i, Wif is for finding f, etc

//----layer 1 weights and biases----//
//weights and biases for i1 (i of layer 1)
double Wii1[W1_Rows][x_size];
double Bii1[W1_Rows];
double Whi1[W1_Rows][H_size];
double Bhi1[W1_Rows];
//f1
double Wif1[W1_Rows][x_size];
double Bif1[W1_Rows];
double Whf1[W1_Rows][H_size];
double Bhf1[W1_Rows];
//g1
double Wig1[W1_Rows][x_size];
double Big1[W1_Rows];
double Whg1[W1_Rows][H_size];
double Bhg1[W1_Rows];
//o1
double Wio1[W1_Rows][x_size];
double Bio1[W1_Rows];
double Who1[W1_Rows][H_size];
double Bho1[W1_Rows];

//conversion variable (turns Ht-1 doubleo predicted x for next input)
double Bernoulli1[x_size][W1_Rows];

//----layer 2 weights and biases----//
//i2
double Wii2[W2_Rows][x_size];
double Bii2[W2_Rows];
double Whi2[W2_Rows][H_size];
double Bhi2[W2_Rows];
//f2
double Wif2[W2_Rows][x_size];
double Bif2[W2_Rows];
double Whf2[W2_Rows][H_size];
double Bhf2[W2_Rows];
//g2
double Wig2[W2_Rows][x_size];
double Big2[W2_Rows];
double Whg2[W2_Rows][H_size];
double Bhg2[W2_Rows];
//o2
double Wio2[W2_Rows][x_size];
double Bio2[W2_Rows];
double Who2[W2_Rows][H_size];
double Bho2[W2_Rows];

//----Fully Connected Layer----//
double WFC[WFC_Rows][W2_Rows];
double BFC[WFC_Rows];

void initialize_lstm128(void *param){
    srand(3);
    double highestFloat = 10.0f;
    for(int i=0; i<x_size; i++){
        X[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
    }
    for(int i=0; i<H_size; i++){
        H_l1[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
        H_l2[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
    }
    for(int i=0; i<C_size; i++){
        C_l1[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
        C_l2[i] = (double)rand()/(double)(RAND_MAX/highestFloat);
    }
    for(int i=0; i<W1_Rows; i++)
    {
        Bii1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bif1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Big1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bio1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<x_size; j++)
        {
            Wii1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wif1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wig1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wio1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    for(int i=0; i<W1_Rows; i++)
    {
        Bhi1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhf1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhg1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bho1[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<H_size; j++)
        {
            Whi1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whf1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whg1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Who1[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    //dropout
    for(int i=0; i<x_size; i++)
    {
        for(int j=0; j<W1_Rows; j++)
        {
            int r = rand()%2;
            if(r==1){Bernoulli1[i][j] = 1.0f;}
            else {Bernoulli1[i][j] = 0.0f;}
            
        }
    }

    for(int i=0; i<W2_Rows; i++)
    {
        Bii2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bif2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Big2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bio2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<x_size; j++)
        {
            Wii2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wif2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wig2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Wio2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    for(int i=0; i<W2_Rows; i++)
    {
        Bhi2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhf2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bhg2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        Bho2[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<W1_Rows; j++)
        {
            Whi2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whf2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Whg2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
            Who2[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    for(int i=0; i<WFC_Rows; i++)
    {
        BFC[i]=(double)rand()/(double)(RAND_MAX/highestFloat);
        for(int j=0; j<W2_Rows; j++)
        {
            WFC[i][j]=(double)rand()/(double)(RAND_MAX/highestFloat);
        }
    }
    return;
}

void populate_lstm128_1s(){
    for(int i=0; i<x_size; i++){
        X[i] = 1.0f;
    }
    for(int i=0; i<H_size; i++){
        H_l1[i] = 1.0f;
    }
    for(int i=0; i<C_size; i++){
        C_l1[i] = 1.0f;
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
    write_array_to_file(outfile, X, x_size, "Input", 0);
    write_array_to_file(outfile, H_l1, H_size, "H_l1", 0);
    write_array_to_file(outfile, C_l1, C_size, "C_l1", 0);
    write_array_to_file(outfile, H_l2, H_size, "H_l2", 0);
    write_array_to_file(outfile, C_l2, C_size, "C_l2", 0);
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
    lstm128_to_json("../WeightsAndBiases/lstm128.json");
    //void lstm_gate(x,Wi,Bi,h,Wh,Bh,x_size,h_size,output_size,act_func,temp1,temp2,temp3,des)
    
    //-----------------------Layer 1--------------------------//
    load_v(1, X, x_size); //x1 in reg 1
    load_v(2, C_l1, C_size); //C1 in reg 2
    load_v(3, H_l1, H_size); //h1 in reg 3

    load_m(4, Wii1, W1_Rows, x_size);
    load_v_t(5, Bii1, W1_Rows);
    load_m(6, Whi1, W1_Rows, H_size);
    load_v_t(7, Bif1, W1_Rows);

    //i in register 7. 29, 30, and 31 are for temporary registers
    //void   (x,Wi,Bi, h,Wh,Bh, x_size, h_size, output_size, int act_func, reg temp1,reg temp2,reg temp3, reg des){
    lstm_gate(1,4,5, 3,6,7, x_size, H_size, W1_Rows, 1, 29,30,31, 8); //i1 in 8

    //calculate f1
    load_m(4, Wif1, W1_Rows, x_size);
    load_v_t(5, Bif1, W1_Rows);
    load_m(6, Whf1, W1_Rows, H_size);
    load_v_t(7, Bhf1, W1_Rows);
    lstm_gate(1,4,5, 3,6,7, x_size, H_size, W1_Rows, 1, 29,30,31, 9); //f1 in reg 9

    //calculate g
    load_m(4, Wig1, W1_Rows, x_size);
    load_v_t(5, Big1, W1_Rows);
    load_m(6, Whg1, W1_Rows, H_size);
    load_v_t(7, Bhg1, W1_Rows);                  // vvv for tanh
    lstm_gate(1,4,5, 3,6,7, x_size, H_size, W1_Rows, 2, 29, 30, 31, 10); //g1 in reg 10
    //calculate o
    load_m(4, Wio1, W1_Rows, x_size);
    load_v_t(5, Bio1, W1_Rows);
    load_m(6, Who1, W1_Rows, H_size);
    load_v_t(7, Bho1, W1_Rows);
    lstm_gate(1,4,5, 3,6,7, x_size, H_size, W1_Rows, 1, 29, 30, 31, 11); //o1 in reg 11
    //calculate C1
   
    e_mul(9, 2, 13); //f1*c1 in reg 13
    e_mul(8, 10, 14);//i1*g1 in reg 14 
    add(13, 14, 2); //overwrite with new C1 in reg 2

    //calculate H1
    rotate_mov(2, 13); //setup C1 copy for activation
    activation(13, 14, 2); //apply tanh to c1
    e_mul(11, 14, 3); //H1 = o1 * tanh(c1)

    //predict x_layer2
    load_m(13, Bernoulli1, x_size, W1_Rows);
    e_mul_mv(13, 3, x_size, W1_Rows, 14); //h1*bernoulli
    acc_col(14, x_size, W1_Rows, 0, 16); //overwrite x2 with pred-x2
    rotate(16); //predicted x2 in 16

    //-----------Layer 2----------//
    //x_layer2 in 16, c2 in 17, h2 in 18
    load_v(17, C_l2, C_size); //C2
    load_v(18, H_l2, H_size); //H2
    load_m(4, Wii2, W1_Rows, x_size);
    load_v_t(5, Bii2, W1_Rows);
    load_m(6, Whi2, W1_Rows, W1_Rows);
    load_v_t(7, Bif2, W1_Rows);
    //calculate i
    //void   (x,Wi,Bi, h,Wh,Bh, x_size, h_size, output_size, int act_func, reg temp1,reg temp2,reg temp3, reg des)
    lstm_gate(16,4,5, 18,6,7, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 8); //i2 in 8
    // printreg_segment(7, 1, 128);

    //calculate f2
    load_m(4, Wif2, W2_Rows, x_size);
    load_v_t(5, Bif2, W2_Rows);
    load_m(6, Whf2, W2_Rows, W1_Rows);
    load_v_t(7, Bhf2, W2_Rows);
    lstm_gate(16,4,5, 18,6,7, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 9); //f2 in reg 9
    // printreg_segment(8, 1, 128);
    //calculate g2
    load_m(4, Wig2, W2_Rows, x_size);
    load_v_t(5, Big2, W2_Rows);
    load_m(6, Whg2, W2_Rows, W1_Rows);
    load_v_t(7, Bhg2, W2_Rows);                  // vvv for tanh
    lstm_gate(16,4,5, 18,6,7, x_size, W1_Rows, W2_Rows, 2, 29, 30, 31, 10); //g2 in reg 10
    // printreg_segment(9, 1, 128);

    //calculate o2
    load_m(4, Wio2, W2_Rows, x_size);
    load_v_t(5, Bio2, W2_Rows);
    load_m(6, Who2, W2_Rows, W1_Rows);
    load_v_t(7, Bho2, W2_Rows);
    lstm_gate(16,4,5, 18,6,7, x_size, W1_Rows, W2_Rows, 1, 29, 30, 31, 11); //o2 in reg 11
    // printreg_segment(10, 1, 128);

    //calculate C2
    e_mul(9, 17, 13); //f2*c1 in reg 13
    e_mul(8, 10, 14);//i2*g2 in reg 14 
    add(13, 14, 17); //overwrite old c2 with new C2 in 17
    // printreg_segment(12, 1, 128);

    //calculate H2
    rotate_mov(17, 13); //setup C2 copy for activation
    activation(13, 14, 2); //apply tanh to c2
    e_mul(11, 14, 18); //H2 = tanh(c2) * o2. reg 18

    //fully connected layer using just h2 for now
    load_m(19, WFC, WFC_Rows, W2_Rows);
    // printreg_segment(1, 65, 128);
    e_mul_mv(19, 18, WFC_Rows, W2_Rows, 20);
    acc_col(20, WFC_Rows, W2_Rows, 0, 21);
    // printreg_segment(4, 1, 65);
    load_v_t(22, BFC, WFC_Rows);

    add(21, 22, 23);
    rotate(23);
    // printreg_segment(5, 1, 65);
    printreg_to_file(23, 1, 65, "../outputs/lstm128_c_output.txt");
    return 0;
}

