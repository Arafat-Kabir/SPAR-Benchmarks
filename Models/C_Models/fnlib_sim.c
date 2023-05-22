#include "fnlib.h"
#include <stdio.h>

#define array_size 784

static double registers[32][(array_size*array_size)];

void load_m(reg a, void * b, int rows, int cols) {
    double *mat =  (double *)b;
    
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
            registers[result][i+(j*array_size)]=registers[m][i+(j*array_size)]*registers[v][i];
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

void ReLU(reg a, reg des){
    for(int i=0; i<array_size*array_size; i++)
    {
        if(registers[a][i]<0){registers[des][i] = 0;}
        else {registers[des][i] = registers[a][i]; }
    }
}

void rotate(reg a){
    double temp = 0.0;
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

void printreg_v(reg a){
    for(int i=0; i<array_size; i++)
    {
        printf("%f, ",registers[a][i]);
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
            fprintf(out_file, "%.4f,\n", registers[a][(i*array_size)+j]);
        }
        
    }
}

void lstm_gate(reg x, reg Wi, reg Bi, reg h, reg Wh, reg Bh, int x_size, int h_size, int output_size, int act_func, reg temp, reg des){
    
    return;
}