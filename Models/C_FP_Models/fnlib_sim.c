#include "fnlib.h"
#include "fplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define array_size 784
#define EULER_NUMBER 2.71828

static fixp registers[32][(array_size*array_size)];

void load_m(reg a, void * b, int rows, int cols) {
    fixp *mat =  (fixp *)b;
    
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            registers[a][j+(i*array_size)] = mat[j+(i*cols)];
        }
    }
    printf("load from simulation with %d columns and %d rows\r\n", cols, rows);
}

void load_v(reg a, vec b, int size){
    for(int i=0; i<size; i++)
    {
        registers[a][i] = b[i];
    }
    printf("load vector from simulation of size %d \r\n", size);
}

void load_v_t(reg a, vec b, int size){
    for(int i=0; i<size; i++)
    {
        registers[a][i*array_size] = b[i];
    }
    printf("load vector turned from simulation of size %d \r\n", size);
}

void printreg(reg a){
    for(int i=0; i<array_size; i++) //rows
    {
        for(int j=0; j<array_size; j++)//columns
        {
            // if(registers[a][j+(i*array_size)]>0.00f){
            printf("%.1f, ", registers[a][j+(i*array_size)]);
            // }
        }
        printf("\n\n");
    }
}

//multiplies vector along every row.
void e_mul_mv(reg m, reg v, int rowm, int colm, reg result){
    for(int i=0; i<colm; i++)
    {
        for(int j=0; j<rowm; j++)
        {
            registers[result][i+(j*array_size)]=
            fp_mul(registers[m][i+(j*array_size)],registers[v][i]);
            // printf("%.4f, %.4f, %.4f \n", registers[v][j], registers[m][i+(j*array_size)], registers[result][i+(j*array_size)]);
        }
    }
}

void acc_col(reg m, int rowm, int colm, reg temp, reg result){
    printf("Accumulating %d columns for reg %d\r\n", colm, m);
    for(int j=0; j<rowm; j++)
    {
        registers[result][(j*array_size)]=0;
        for(int i=0; i<colm; i++)
        {
            registers[result][(j*array_size)] += registers[m][(j*array_size)+i];
            // printf("%f, ", registers[m][(j*colm)+i]);
        }
        // printf("%.4f\n",registers[result][(j*array_size)]);
    }
}

void add(reg a, reg b, reg des){
    for(int i=0; i<array_size*array_size; i++)
    {
        registers[des][i] = registers[a][i]+registers[b][i];
    }
}

void e_mul(reg a, reg b, reg result){
    for(int i=0; i<(array_size*array_size); i++)
    {
        registers[result][i]=fp_mul(registers[a][i], registers[b][i]);//(registers[a][i] * registers[b][i];
    }
    return;
}


void ReLU(reg a, reg des){
    for(int i=0; i<array_size*array_size; i++)
    {
        if(registers[a][i]<0){registers[des][i] = 0;}
        else {registers[des][i] = registers[a][i]; }
    }
}

void rotate(reg a){
    fixp temp = 0.0;
    for(int i=0; i<array_size; i++)
    {
        for(int j=i+1; j<array_size; j++)
        {
            temp = registers[a][i+(j*array_size)];
            registers[a][i+(j*array_size)] = registers[a][j+(i*array_size)];
            registers[a][j+(i*array_size)] = temp;
        }
    }
}

void rotate_mov(reg a, reg des){
    for(int i=0; i<array_size; i++)
    {
        for(int j=0; j<array_size; j++)
        {
            registers[des][i+(j*array_size)] = registers[a][j+(i*array_size)];
        }
    }
}

void printreg_v(reg a){
    for(int i=0; i<array_size; i++)
    {
        printf("%f,\n",registers[a][i]);
    }
    printf("\n");
}

void printreg_to_file(reg a, int row, int col, char *filename)
{
    FILE *out_file = fopen(filename, "w");
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            int32_t number = registers[a][(i*array_size)+j];
            int32_t fraction = (100000*((int64_t)number&(int64_t)(0xFFFF)))/((int64_t)1<<16);
            // printf("%d\n", number);
            if(number < 0 && fraction > 0) {
                // printf("fixing\n");
                fraction = 100000 - fraction;
                fprintf(out_file, "%d.%d, ", (number>>16)+1, fraction);
            }
            else{
                fprintf(out_file, "%d.%d, ", number>>16, fraction);
            }
        }
    }
}

void lstm_gate(reg x, reg Wi, reg Bi, reg h, reg Wh, reg Bh, int x_size, int h_size, int output_size, int act_func, reg temp1, reg temp2, reg temp3, reg des){
    //Multiply accumulate add for x
    e_mul_mv(Wi, x, output_size, x_size, temp1);
    acc_col(temp1, output_size, x_size, temp2, des);
    add(des, Bi, des); //Assume bias is automatically oriented
    //multiply accumulate add for h
    e_mul_mv(Wh, h, output_size, h_size, temp1);
    acc_col(temp1, output_size, h_size, temp2, temp3);
    add(temp3, Bh, temp3);
    //Add the two
    add(temp3, des, temp3);
    //apply a non-ReLU activation function that also rotates the vector
    activation(temp3, des, act_func);
    return;
}

void activation(reg a, reg des, acttype act){
    if(act == 1) //sigmoid
    {
        for(int i=0; i<array_size; i++) //go along the first column; write to first row
        {
            registers[des][i] = (1.0/(1.0 + pow(EULER_NUMBER, -1.0 * registers[a][i*array_size])));
        }
    }
    if(act == 2) //sigmoid
    {
        for(int i=0; i<array_size; i++) //go along the first column; write to first row
        {
            registers[des][i] = tanh(registers[a][i*array_size]);
        }
    }
    return;
}

void printreg_segment(reg a, int row, int col){
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<col; j++)
        {
            int32_t number = registers[a][(i*array_size)+j];
            int32_t fraction = (100000*((int64_t)number&(int64_t)(0xFFFF)))/((int64_t)1<<16);
            // printf("%d\n", number);
            if(number < 0 && fraction > 0) {
                // printf("fixing\n");
                fraction = 100000 - fraction;
                printf("%d.%d, ", (number>>16)+1, fraction);
            }
            else{
                printf("%d.%d, ", number>>16, fraction);
            }
        }
        printf("\n");
    }
}

void write_array_to_file(FILE *outfile, void *array, long size, char *name, int end){ //append an array to the end of a file
    double *arr = (double *) array;
    if(outfile==NULL) return;
    else{
        fprintf(outfile, "\"%s\": [",name);
        for(int i=0; i<size; i++)
        {
            fprintf(outfile, "%lf", arr[i]);
            if(i<size-1){fprintf(outfile, ", ");}
            if(i%20==19){fprintf(outfile, "\n");}
        }
        if(end==0){fprintf(outfile, "],\n");}
        else {fprintf(outfile, "]\n");}
    }
}