#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "platform.h"
//#include "ARPMB.h"
//#include "xparameters.h"
//#include "microblaze_sleep.h"
//#include "xtmrctr.h"
#include <inttypes.h>
//#include "xbasic_types.h"
#include "math.h"


#define MAX_LEN 8
#define FIXED_POINT_FRACTIONAL_BITS 4

int ShL_cnt = 0;
int ShR_cnt = 0;
int ShN_cnt = 0;
int ShS_cnt = 0;
int mult_cnt = 0;
int add_cnt = 0;
int write_stall_cnt = 0;
int read_stall_cnt = 0;

//Function prototypes
int execute(int opcode, int rd, int rs1, int rs2);
int readRAM(int address);
int getbit(int n, int k);
void getTranspose(int* ram, int* reg);
void printArray(int* arr, int size);
void getData(int* arr, int address);
void printRegFile();
void WRITE(int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB);
int READ(int BRAM_i, int BRAM_j, int ADDRA, int ADDRB);
unsigned int READ_REG(int BRAM_i, int BRAM_j, int PE, int reg);
int printReg(int BRAM_i, int BRAM_j, int PE, int reg);
int getTrans(int* ram);
void printRegFile(int BRAM_i, int BRAM_j, int number_of_regs);
void WRITE_REG(int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data);

//Fixed-point
typedef int32_t fixed_point_t;
fixed_point_t float_to_fixed(double input);

int LEN = MAX_LEN << 24;

#define block_dim 28
#define batch 1
#define nodes_l1 3
#define nodes_l2 64
#define nodes_l3 16
#define nodes_l4 64
#define nodes_l5 64
#define nodes_l6 16
#define nodes_l7 64
#define nodes_l8 64
#define nodes_l9 32
#define nodes_l10 128
#define nodes_l11 128
#define nodes_l12 32
#define nodes_l13 128
#define nodes_l14 128
#define nodes_l15 48
#define nodes_l16 192
#define nodes_l17 192
#define nodes_l18 48
#define nodes_l19 192
#define nodes_l20 192
#define nodes_l21 64
#define nodes_l22 256
#define nodes_l23 256
#define nodes_l24 64
#define nodes_l25 256
#define nodes_l26 256
#define nodes_l27 10

#define kernel_w_1 3
#define kernel_h_1 3
#define kernel_w_2 1
#define kernel_h_2 1
#define kernel_w_3 1
#define kernel_h_3 1
#define kernel_w_4 3
#define kernel_h_4 3
#define kernel_w_5 1
#define kernel_h_5 1
#define kernel_w_6 1
#define kernel_h_6 1
#define kernel_w_7 3
#define kernel_h_7 3
#define kernel_w_8 1
#define kernel_h_8 1
#define kernel_w_9 1
#define kernel_h_9 1
#define kernel_w_10 3
#define kernel_h_10 3
#define kernel_w_11 1
#define kernel_h_11 1
#define kernel_w_12 1
#define kernel_h_12 1
#define kernel_w_13 3
#define kernel_h_13 3
#define kernel_w_14 1
#define kernel_h_14 1
#define kernel_w_15 1
#define kernel_h_15 1
#define kernel_w_16 3
#define kernel_h_16 3
#define kernel_w_17 1
#define kernel_h_17 1
#define kernel_w_18 1
#define kernel_h_18 1
#define kernel_w_19 3
#define kernel_h_19 3
#define kernel_w_20 1
#define kernel_h_20 1
#define kernel_w_21 1
#define kernel_h_21 1
#define kernel_w_22 3
#define kernel_h_22 3
#define kernel_w_23 1
#define kernel_h_23 1
#define kernel_w_24 1
#define kernel_h_24 1
#define kernel_w_25 3
#define kernel_h_25 3
#define kernel_w_26 1
#define kernel_h_26 1

#define kernel_stride_1 2
#define kernel_stride_2 1
#define kernel_stride_3 1
#define kernel_stride_4 1
#define kernel_stride_5 1
#define kernel_stride_6 1
#define kernel_stride_7 1
#define kernel_stride_8 1
#define kernel_stride_9 1
#define kernel_stride_10 1
#define kernel_stride_11 1
#define kernel_stride_12 1
#define kernel_stride_13 1
#define kernel_stride_14 1
#define kernel_stride_15 1
#define kernel_stride_16 1
#define kernel_stride_17 1
#define kernel_stride_18 1
#define kernel_stride_19 1
#define kernel_stride_20 1
#define kernel_stride_21 1
#define kernel_stride_22 1
#define kernel_stride_23 1
#define kernel_stride_24 1
#define kernel_stride_25 1
#define kernel_stride_26 1

#define kernel_padding_1 0
#define kernel_padding_2 0
#define kernel_padding_3 0
#define kernel_padding_4 1
#define kernel_padding_5 0
#define kernel_padding_6 0
#define kernel_padding_7 1
#define kernel_padding_8 0
#define kernel_padding_9 0
#define kernel_padding_10 1
#define kernel_padding_11 0
#define kernel_padding_12 0
#define kernel_padding_13 1
#define kernel_padding_14 0
#define kernel_padding_15 0
#define kernel_padding_16 1
#define kernel_padding_17 0
#define kernel_padding_18 0
#define kernel_padding_19 1
#define kernel_padding_20 0
#define kernel_padding_21 0
#define kernel_padding_22 1
#define kernel_padding_23 0
#define kernel_padding_24 0
#define kernel_padding_25 1
#define kernel_padding_26 0

#define pool_w_1 3
#define pool_h_1 3
#define pool_s_1 2
#define pool_p_1 0

#define pool_w_2 3
#define pool_h_2 3
#define pool_s_2 2
#define pool_p_2 0

#define pool_w_3 3
#define pool_h_3 3
#define pool_s_3 2
#define pool_p_3 0

#define input_w_1 224
#define input_h_1 224
#define out_Width_0 224
#define out_Hight_0 224
#define out_Width_1 111 			//(int)((input_w - kernel_w + 2*padding) / stride) + 1;
#define out_Hight_1 111 			//(int)((input_h - kernel_h + 2*padding) / stride) + 1;
#define out_pool_Width_1 55 		//(int)((out_Width + (2*padding) - (kernel_w - 1) - 1) / stride) + 1;
#define out_pool_Hight_1 55 		//(int)((out_Hight + (2*padding) - (kernel_h - 1) - 1) / stride) + 1;
#define out_Width_2 55
#define out_Hight_2 55
#define out_Width_3 55
#define out_Hight_3 55
#define out_Width_4 55
#define out_Hight_4 55
#define out_Width_5 55
#define out_Hight_5 55
#define out_Width_6 55
#define out_Hight_6 55
#define out_Width_7 55
#define out_Hight_7 55
#define out_pool_Width_2 27
#define out_pool_Hight_2 27
#define out_Width_8 27
#define out_Hight_8 27
#define out_Width_9 27
#define out_Hight_9 27
#define out_Width_10 27
#define out_Hight_10 27
#define out_Width_11 27
#define out_Hight_11 27
#define out_Width_12 27
#define out_Hight_12 27
#define out_Width_13 27
#define out_Hight_13 27
#define out_pool_Width_3 13
#define out_pool_Hight_3 13
#define out_Width_14 13
#define out_Hight_14 13
#define out_Width_15 13
#define out_Hight_15 13
#define out_Width_16 13
#define out_Hight_16 13
#define out_Width_17 13
#define out_Hight_17 13
#define out_Width_18 13
#define out_Hight_18 13
#define out_Width_19 13
#define out_Hight_19 13
#define out_Width_20 13
#define out_Hight_20 13
#define out_Width_21 13
#define out_Hight_21 13
#define out_Width_22 13
#define out_Hight_22 13
#define out_Width_23 13
#define out_Hight_23 13
#define out_Width_24 13
#define out_Hight_24 13
#define out_Width_25 13
#define out_Hight_25 13
#define out_Width_26 13
#define out_Hight_26 13

void ColumnToRow(int rd, int rs, int function){

	//function: 0 == noOp, 1 == sigmoid, 2 == tanh
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG6_OFFSET, function);
	execute(5, rs, rs, 0);
	ShR_cnt++;
	int instruction = (7<<26) + (rd<<21) + (rd << 16) + (0 << 11);

//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	ShS_cnt++;
}

void RowToRow(int rd, int rs){

//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG6_OFFSET, 0x6);
	execute(8, rs, rs, 0);
	ShN_cnt++;
	int instruction = (7<<26) + (rd<<21) + (rd << 16) + (0 << 11);

//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	ShS_cnt++;
}

void print2DArray(int rows, int cols, int arr[][cols]){

	int i, j;
	for( i=0; i<rows; i++){
		for( j=0; j<cols; j++){
			printf("%08x\t", arr[i][j]);
		}
		printf("\n\r");
	}
}

inline fixed_point_t float_to_fixed(double input)
{
    return (fixed_point_t)(input * (1 << FIXED_POINT_FRACTIONAL_BITS));
}

void FxP_Conversion_1D(int t1, float in[], int out[]){

	int i;
	for( i = 0; i < t1; i++)
	{
		out[i] = float_to_fixed(in[i]);
	}
}

void FxP_Conversion_4D(int t1, int t2, int t3, int t4, float in[][t2][t3][t4], int out[][t2][t3][t4]){

	int i, j, k, l;
	for( i = 0; i < t1; i++)
	{
		for( j = 0; j < t2; j++)
		{
			for( k = 0; k < t3; k++)
			{
				for( l = 0; l < t4; l++)
				{
					out[i][j][k][l] = float_to_fixed(in[i][j][k][l]);
				}
			}
		}
	}
}

void ELEMENTWISE_MULTIPLICATION(int matrixA_reg, int matrixB_reg, int Result_Reg){

	execute(2,Result_Reg,matrixA_reg,matrixB_reg);
	mult_cnt++;
}

void MARTIX_ADDITION(int matrixA_reg, int matrixB_reg, int Result_Reg){

	execute(0,Result_Reg,matrixA_reg,matrixB_reg);
	add_cnt++;
}

void MARTIX_MULTIPLICATION(int matrixA_reg, int matrixB_reg, int Result_Reg, int matrixA_col, int block_dimension){

	execute(2,21,matrixB_reg,matrixA_reg);	//Mult
	mult_cnt++;
	execute(5,27,21,0);						//ShiftEast
	ShR_cnt++;
	execute(0,22,27,21);					//Add
	add_cnt++;
	//shift and add
	int i;
	for( i = 0; i < matrixA_col-2; i++)
	{
		execute(5,23,22,0);					//ShiftEast
		ShR_cnt++;
		execute(0,22,23,21);				//Add
		add_cnt++;
	}
	//shift till edge
	for( i = 0; i < (block_dimension*4) - matrixA_col; i++)
	{
		execute(5,22,22,0);					//ShiftEast
		ShR_cnt++;
	}
	execute(0,Result_Reg,22,0);
	add_cnt++;
	return;

}

unsigned int Log2n(unsigned int n)
{
    return (n > 1) ? 1 + Log2n(n / 2) : 0;
}

void MARTIX_MULTIPLICATION_Optimized(int matrixA_reg, int matrixB_reg, int Result_Reg, int matrixA_col, int block_dimension){

	unsigned int log_colA = Log2n(matrixA_col);
	int col_cnt = 0;
	execute(2,21,matrixB_reg,matrixA_reg);	//Mult
	mult_cnt++;
	execute(0,23,21,0);						//Add
	add_cnt++;
	int i, j;
	for( i=0; i<log_colA; i++){
		execute(5,22,21,0);						//ShiftEast
		col_cnt++;
		ShR_cnt++;
		for( j=0; j<pow((double)2,(double)i)-1; j++){
			execute(5,22,22,0);						//ShiftEast
			col_cnt++;
			ShR_cnt++;
		}
		execute(0,21,21,22);					//Add
		add_cnt++;
	}
	if(col_cnt != matrixA_col -1)
	{
		//shift and add
		for( i = col_cnt; i < matrixA_col; i++)
		{
			execute(5,22,21,0);					//ShiftEast
			ShR_cnt++;
			execute(0,21,23,22);				//Add
			add_cnt++;
		}
	}

	//shift till edge
	for( i = 0; i < (block_dimension*4) - matrixA_col; i++)
	{
		execute(5,21,21,0);					//ShiftEast
		ShR_cnt++;
	}

	execute(0,Result_Reg,21,0);
	add_cnt++;
	return;

}

void GATE(int rowX, int colX, int rowW, int colW, int rowU, int colU, int rowH, int colH, int rowb, int colb,
		  int X_reg, int W_reg, int U_reg, int H_reg, int b_reg,
		  int block_dimension, int Resultreg, int function){

//	MARTIX_MULTIPLICATION(X_reg, W_reg, 25, colW, block_dimension);		//WX in R25
//	MARTIX_MULTIPLICATION(H_reg, U_reg, 26, colU, block_dimension);		//UH in R26

	MARTIX_MULTIPLICATION_Optimized(X_reg, W_reg, 25, colW, block_dimension);		//WX in R25
	MARTIX_MULTIPLICATION_Optimized(H_reg, U_reg, 26, colU, block_dimension);		//UH in R26
	MARTIX_ADDITION(25, 26, 25);										//WX + UH in R25
	MARTIX_ADDITION(25, b_reg, 25);										//WX + UH + b in R25
	ColumnToRow(Resultreg, 25, function);
}

void WRITE_Matrix_Input(int t1, int t2, int t3, int t4, int in[][t2][t3][t4], int reg)
{
	int i, j;
	for( i = 0; i < t3; i++)
	{
		for( j = 0; j < t4; j++)
		{
			WRITE_REG(i/4, j/4, ((i*4+j)%4+i*4)%16, reg, in[t1][t2][i][j]);
			write_stall_cnt++;
		}
	}
}

void WRITE_Matrix_Bias(int n_l, int out_h, int out_w, int b[], int reg)
{
	int sqr_h_dim = ((block_dim * 4) / (out_h));
	int sqr_w_dim = ((block_dim * 4) / (out_w));
	int block_h_b = (n_l % (sqr_h_dim * sqr_w_dim)) % (sqr_h_dim);
	int block_w_b = (n_l % (sqr_h_dim * sqr_w_dim)) / (sqr_w_dim);
	
	int i, j;
	for( i = (block_h_b * out_h); i < ((block_h_b + 1) * out_h); i++)
	{
		for( j = (block_w_b * out_w); j < ((block_w_b + 1) * out_w); j++)
		{
			WRITE_REG(i/4, j/4, ((i*4+j)%4+i*4)%16, reg, b[n_l]);
			write_stall_cnt++;
		}
	}
	
}

void WRITE_Matrix_Weight(int t1, int t2, int in_h, int in_w, int k_h, int k_w, int pad, int str, int W[][t2][k_h][k_w], int reg)
{
	int sqr_h_dim = ((block_dim * 4) / (in_h));
	int sqr_w_dim = ((block_dim * 4) / (in_w));
	int block_h_b = (t2 % (sqr_h_dim * sqr_w_dim)) % (sqr_h_dim);
	int block_w_b = (t2 % (sqr_h_dim * sqr_w_dim)) / (sqr_w_dim);

	int y, x, y_k, x_k; 
	for( y = block_h_b; y < block_h_b + in_h - k_h + 2 * pad + 1; y = y + str)
	{
		for( x = block_w_b; x < block_w_b + in_w - k_w + 2 * pad + 1; x = x + str)
		{
			for( y_k = 0; y_k < k_h; y_k++)
			{
				for( x_k = 0; x_k < k_w; x_k++)
				{
					WRITE_REG((y + y_k)/4, (x + x_k)/4, (((y + y_k)*4+(x + x_k))%4+(y + y_k)*4)%16, reg, W[t1][t2][y_k][x_k]);
					write_stall_cnt++;
				}
			}

		}
	}
}

void WRITE_Matrix(int row, int col, int W[][col], int reg, int copy, int block_dimension){

	int i, s, t;
	if (copy == 1)		//write a 2D array, starts from the left-most PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(s/4, t/4, ((s*4+t)%4+s*4)%16, reg, W[s][t]);
			}
		}
	}
	else if(copy == 0)	//write a 1D array into last column PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{

				WRITE_REG(s/4, block_dimension-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
	}
	else if(copy == -1)	//write a 1D array into first column PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(0, s/4, s%4, reg, W[s][t]);
			}
		}
	}
	else				//write a 1D array into first row, copies to the below PEs too
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{

				WRITE_REG(s/4, block_dimension-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
		ColumnToRow(reg, reg, 0);

		int instruction = (7<<26) + (reg<<21) + (reg << 16) + (0 << 11);
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
		for( i = 0; i < copy-1; i++)
		{
//			ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//			ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		}

	}

}

int toggleBit(int n, int k){

    return (n ^ (1 << (k - 1)));
}

void WRITE_REG(int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data){
	
	int base = reg*32;
	int i;
	for( i=base-2; i>=base-33; i-=2){

		unsigned int dia = ((data<<(31-i))>>31)<<PE;
		unsigned int dib = ((data<<(31-i-1))>>31)<<PE;
		unsigned int tmp, oldDia, oldDib, newDia, newDib;
		tmp = READ(BRAM_i, BRAM_j, 32+i, 32+i+1);
		oldDia = tmp >> 16;
		oldDib = (tmp << 16) >> 16;
		char oldBitA = getbit(oldDia, PE);
		char oldBitB = getbit(oldDib, PE);
		char BitA = getbit(dia, PE);
		char BitB = getbit(dib, PE);
		newDia = dia|oldDia;
		if(BitA==0 && oldBitA==1) newDia = toggleBit(newDia, PE+1);

		newDib = dib|oldDib;
		if(BitB==0 && oldBitB==1) newDib = toggleBit(newDib, PE+1);

		WRITE(BRAM_i, BRAM_j, 32+i, 32+i+1, newDia, newDib);
	}
}

unsigned int READ_REG(int BRAM_i, int BRAM_j, int PE, int reg){

	int base = reg*32;
	unsigned int out;
	int i;
	for( i=base-2; i>=base-33; i-=2){

		unsigned int tmp, oldDia, oldDib;
		tmp = READ(BRAM_i, BRAM_j, 32+i, 32+i+1);
		oldDia = tmp >> 16;
		oldDib = (tmp << 16) >> 16;
		int oldBitA = getbit(oldDia, PE);
		int oldBitB = getbit(oldDib, PE);
		out = out << 1;
		out = out + oldBitB;
		out = out << 1;
		out = out + oldBitA;
	}	
	return out;
}

void WRITE(int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB){

	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;
	int reg4_input = (DIA << 16) + DIB;

	//BRAM_i = 0, BRAM_j = 0
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG2_OFFSET, reg2_input);

	//ADDRA = 32 , ADDRB = 64
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG3_OFFSET, reg3_input);

	//DIA = 0xFFFF, DIB = 0x1111
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG4_OFFSET, reg4_input);

	//external = 1, WEA = 1, WEB = 1
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x0000001D+LEN);

	//external = 1, WEA = 0, WEB = 0
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG4_OFFSET, 0x00000000);

}

int READ(int BRAM_i, int BRAM_j, int ADDRA, int ADDRB){

	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;

	//BRAM_i = 0, BRAM_j = 0
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG2_OFFSET, reg2_input);

	//ADDRA = 32 , ADDRB = 64
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG3_OFFSET, reg3_input);

	//external = 1, WEA = 0, WEB = 0
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000005+LEN);

	//read
//	int DO = ARPMB_mReadReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG5_OFFSET);

	//external = 1, WEA = 0, WEB = 0
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG3_OFFSET, 0x00000000);

	return 0;
}

int printReg(int BRAM_i, int BRAM_j, int PE, int reg){

	int REG[32];
	int reg_out;
	int DO;
	reg = reg << 5;
	int i;
	for( i=0; i<16; i++){
		DO = READ (BRAM_i, BRAM_j, reg+i, reg+i+16);
		REG[i+16] = getbit(DO, PE);
		REG[i] = getbit(DO, PE+16);
	}

	reg_out = getTrans(REG);

	return reg_out;
}

void printRegFile(int BRAM_i, int BRAM_j, int number_of_regs){

	int PE, i;
	for( PE=0; PE<16; PE++){

		for( i=number_of_regs-1; i>=0; i--)
		{
			printf("%08x \t", printReg(BRAM_i, BRAM_j, PE, i));
		}
		printf("\n\r");

	}

}

int execute(int opcode, int rd, int rs1, int rs2)
{
	int instruction = (opcode<<26) + (rd<<21) + (rs1 << 16) + (rs2 << 11);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);//start = 0, reset = 0
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
//	MB_Sleep(1);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
//	MB_Sleep(1);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//	MB_Sleep(1);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
//	MB_Sleep(1);
//	start_cnt++;
	return 0;
}


int getbit(int n, int k){

	return (n & ( 1 << k )) >> k;
}

void printArray(int* arr, int size){

	int i;
	for(i=0; i<size; i++){
		printf("%x\n\r",arr[i]);
	}
}

void getTranspose(int* ram, int* reg){

	int i, j;
	for(j=0; j<16; j++){
		for(i=31; i>=0; i--){
			reg[j] <<= 1;
			if(getbit(ram[i],j)==1) reg[j]++;
		}
	}
}

int getTrans(int* ram){

	int reg = 0;
	int i;
	for(i=31; i>=0; i--){
		reg <<= 1;
		getbit(ram[i],0)==1? reg++ : reg;
	}
	return reg;

}

void getData(int* arr, int address){

	int i;
	for(i=0; i<32; i++){
//		init_platform();
//		arr[i] = readRAM(address+i);
//		cleanup_platform();
	}
}
int main()
{

//	init_platform();
//	cleanup_platform();

	printf("Program Started...\n\n\r");

//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG0_OFFSET, 0x00000000);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000000);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);

	//reset = 1
//	ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);

	//reset all the registers in the array to zero
	int i, j, addra;
	for( i = 0; i < block_dim; i++){
		for( j = 0; j < block_dim; j++){
			for( addra = 0; addra < 32 * 32; addra++){
				WRITE(i, j, addra, 0, 0, 0);
			}
		}
	}


	// Declaration
	static float W_flp_1 [nodes_l2][nodes_l1][kernel_h_1][kernel_w_1];
	static float b_flp_1 [nodes_l2];
	static float in_flp_1 [batch][nodes_l1][input_h_1][input_w_1];
	static float padded_in_flp_1 [batch][nodes_l1][input_h_1 + 2 * kernel_padding_1][input_w_1 + 2 * kernel_padding_1];

	static int W_fxp_1 [nodes_l2][nodes_l1][kernel_h_1][kernel_w_1];
	static int b_fxp_1 [nodes_l2];
	static int in_fxp_1 [batch][nodes_l1][input_h_1][input_w_1];
	static int padded_in_fxp_1 [batch][nodes_l1][input_h_1 + 2 * kernel_padding_1][input_w_1 + 2 * kernel_padding_1];

	int t1, t2, t3, t4, x, y, x_k, y_k, m, n, k, l, m_pool, n_pool, block_pool_w, block_pool_h, nn, mm;
	int block_conv_h, block_conv_w, block_out_conv_w, block_out_conv_h, block_pool_out_w, block_pool_out_h;
	int arr_full, in_arr_full, out_arr_full;
	int in_block_w, in_block_h, out_block_w, out_block_h;
	int p, q, in_layer;
	static signed int max = -10000;

	WRITE_Matrix_Input(0, 0, input_h_1 / 2 + 2 * kernel_padding_1, input_w_1 / 2 + 2 * kernel_padding_1, padded_in_fxp_1, 60);
	WRITE_Matrix_Input(0, 0, input_h_1 / 2 + 2 * kernel_padding_1, input_w_1 / 2 + 2 * kernel_padding_1, padded_in_fxp_1, 61);
	WRITE_Matrix_Input(0, 0, input_h_1 / 2 + 2 * kernel_padding_1, input_w_1 / 2 + 2 * kernel_padding_1, padded_in_fxp_1, 62);
	WRITE_Matrix_Input(0, 0, input_h_1 / 2 + 2 * kernel_padding_1, input_w_1 / 2 + 2 * kernel_padding_1, padded_in_fxp_1, 63);

	// Initialization
	for (t1 = 0; t1 < nodes_l1; t1++)
	{
		for (t2 = 0; t2 < input_h_1; t2++)
		{
			for (t3 = 0; t3 < input_w_1; t3++)
			{
				in_flp_1[0][t1][t2][t3] = 1;
			}
		}
	}
	for (t1 = 0; t1 < nodes_l2; t1++)
	{
		for (t2 = 0; t2 < nodes_l1; t2++)
		{
			for (t3 = 0; t3 < kernel_h_1; t3++)
			{
				for (t4 = 0; t4 < kernel_w_1; t4++)
				{
					W_flp_1[t1][t2][t3][t4] = 0.05;
				}
			}
		}
	}

	for (t1 = 0; t1 < nodes_l2; t1++)
	{
		b_flp_1[t1] = 0.2;
	}
	printf("Initialization 1 is done \r\n");

	// Input Padding
	for (t1 = 0; t1 < nodes_l1; t1++)
	{
		for (t2 = 0; t2 < input_h_1 + 2 * kernel_padding_1; t2++)
		{
			for (t3 = 0; t3 < input_w_1 + 2 * kernel_padding_1; t3++)
			{
				if(t2 < kernel_padding_1 || t2 >= (input_h_1 + kernel_padding_1))
				{
					padded_in_flp_1[0][t1][t2][t3] = 0;
				}
				else if(t3 < kernel_padding_1 || t3 >= (input_w_1 + kernel_padding_1))
				{
					padded_in_flp_1[0][t1][t2][t3] = 0;
				}
				else
				{
					padded_in_flp_1[0][t1][t2][t3] = in_flp_1[0][t1][t2 - kernel_padding_1][t3 - kernel_padding_1];
				}
			}
		}
	}
	printf("Input Padding 1 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_4D(nodes_l2, nodes_l1, kernel_h_1, kernel_w_1, W_flp_1, W_fxp_1);
	FxP_Conversion_1D(nodes_l2, b_flp_1, b_fxp_1);
	FxP_Conversion_4D(batch, nodes_l1, input_h_1, input_w_1, in_flp_1, in_fxp_1);
	FxP_Conversion_4D(batch, nodes_l1, input_h_1 + 2 * kernel_padding_1, input_w_1 + 2 * kernel_padding_1, padded_in_flp_1, padded_in_fxp_1);
	printf("Fixed_Point Conversion 1 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 60;
	out_arr_full = 0;
	in_layer = 4;
	printf("Reseting 1 is done \r\n");

	for( j = 0; j < nodes_l2; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l1; i++)
		{
			//WRITE_Matrix_Weight(j, i, out_Hight_0, out_Width_0, kernel_h_1, kernel_w_1, kernel_padding_1, kernel_stride_1, W_fxp_1, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 1 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_1 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_1 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 1 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l1; i++)
		{
			if(in_block_w == 1 && in_block_h == 1)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_0; p++)
			{
				for( q = 0; q < out_Hight_0; q++)
				{
					if((out_Width_0 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_0 * in_block_h);
						mm = p + (out_Width_0 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 1 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_0 - 1) && (q == out_Hight_0 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_0 - 1) && (q == out_Hight_0 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_0 * in_block_h);
						mm = p + (out_Width_0 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 1 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 1 + in_arr_full, 14, 1 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_1 - 1; n < out_Hight_0 + kernel_stride_1; n = n + kernel_stride_1)
		{
			p = 0;
			for( m = kernel_w_1 - 1; m < out_Width_0 + kernel_stride_1; m = m + kernel_stride_1)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 1 + in_arr_full);
				if((out_Width_1 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_1 * out_block_h);
					k = p + (out_Width_1 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_1 - 1) && (q == out_Hight_1 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_1 - 1) && (q == out_Hight_1 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_1 * out_block_h);
					k = p + (out_Width_1 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_1, out_Width_1, b_fxp_1, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

		// Max pooling
		m_pool = 0;
		n_pool = 0;
		nn = 0;
		mm = 0;
		if(block_pool_w == 1 && block_pool_h == 1)
		{
			arr_full++;
			block_pool_w = 0;
			block_pool_h = 0;
		}
		for( y = (block_pool_h * out_Hight_1); y < ((block_pool_h + 1) * out_Hight_1) - pool_h_1 + 2 * pool_p_1 + 1; y = y + pool_s_1)
		{
			for( x = (block_pool_h * out_Width_1); x < ((block_pool_w + 1) * out_Width_1) - pool_w_1 + 2 * pool_p_1 + 1; x = x + pool_s_1)
			{
				for( y_k = y; y_k < y + pool_h_1; y_k++)
				{
					for( x_k = x; x_k < x + pool_w_1; x_k++)
					{
						signed int tmp_out = READ_REG(y_k/4, x_k/4, ((y_k*4+x_k)%4+y_k*4)%16, 17 + out_arr_full);
						if (tmp_out > max)
						{
							max = tmp_out;
						}
					}
				}
				// write max
				if((out_pool_Width_1 * (block_pool_w + 1)) < block_dim * 4)
				{
					nn = n_pool + (out_pool_Hight_1 * block_pool_h);
					mm = m_pool + (out_pool_Width_1 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + out_arr_full, max);

					if((m_pool == out_pool_Width_1 - 1) && (n_pool == out_pool_Hight_1 - 1))
					{
						block_pool_w++;
					}
				}
				else
				{
					if((m_pool == out_pool_Width_1 - 1) && (n_pool == out_pool_Hight_1 - 1))
					{
						block_pool_h++;
						block_pool_w = 0;
					}

					nn = n_pool + (out_pool_Hight_1 * block_pool_h);
					mm = m_pool + (out_pool_Width_1 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + out_arr_full, max);
				}


				m_pool++;
				if (m_pool == out_pool_Width_1)
				{
					m_pool = 0;
					n_pool++;
				}
				max = -100000000;

			}
		}
	} //end nodes_l2 in R17-R32 (64, 55, 55)

	//Move output of prev layer to input of current layer
	//Not needed yet

	//fire2
	// Declaration
	static float W_flp_2 [nodes_l3][nodes_l2][kernel_h_2][kernel_w_2];
	static float b_flp_2 [nodes_l3];

	static int W_fxp_2 [nodes_l3][nodes_l2][kernel_h_2][kernel_w_2];
	static int b_fxp_2 [nodes_l3];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l3; t1++)
	{
		for (t2 = 0; t2 < nodes_l2; t2++)
		{
			for (t3 = 0; t3 < kernel_h_2; t3++)
			{
				for (t4 = 0; t4 < kernel_w_2; t4++)
				{
					W_flp_2[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l3; t1++)
	{
		b_flp_2[t1] = 0.1;
	}
	printf("Initialization 2 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l3, b_flp_2, b_fxp_2);
	FxP_Conversion_4D(nodes_l3, nodes_l2, kernel_h_2, kernel_w_2, W_flp_2, W_fxp_2);
	printf("Fixed_Point Conversion 2 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 16;
	in_layer = 16;
	printf("Reseting 2 is done \r\n");

	for( j = 0; j < nodes_l3; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l2; i++)
		{
			//WRITE_Matrix_Weight(j, i, out_Hight_1, out_Width_1, kernel_h_2, kernel_w_2, kernel_padding_2, kernel_stride_2, W_fxp_2, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_1 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_1 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l2; i++)
		{
			if(in_block_w == 1 && in_block_h == 1)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_1; p++)
			{
				for( q = 0; q < out_Hight_1; q++)
				{
					if((out_Width_1 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_1 * in_block_h);
						mm = p + (out_Width_1 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_1 - 1) && (q == out_Hight_1 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_1 - 1) && (q == out_Hight_1 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_1 * in_block_h);
						mm = p + (out_Width_1 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_2 - 1; n < out_Hight_1 + kernel_stride_2; n = n + kernel_stride_2)
		{
			p = 0;
			for( m = kernel_w_2 - 1; m < out_Width_1 + kernel_stride_2; m = m + kernel_stride_2)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_2 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_4)) < block_dim * 4)
				{
					l = q + (out_Hight_2 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_4);
					k = p + (out_Width_2 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_4);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_2 - 1) && (q == out_Hight_2 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_2 - 1) && (q == out_Hight_2 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_2 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_4);
					k = p + (out_Width_2 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_4);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_2, out_Width_2, b_fxp_1, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	}//end nodes_l3 in R33-R36 (16, 55, 55)

	// Moving output to the next input layer
	execute(0, 17, 33, 0);
	execute(0, 18, 34, 0);
	execute(0, 19, 35, 0);
	execute(0, 20, 36, 0);

	// Declaration
	static float W_flp_3 [nodes_l4][nodes_l3][kernel_h_3][kernel_w_3];
	static float b_flp_3 [nodes_l4];

	static int W_fxp_3 [nodes_l4][nodes_l3][kernel_h_3][kernel_w_3];
	static int b_fxp_3 [nodes_l4];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l4; t1++)
	{
		for (t2 = 0; t2 < nodes_l3; t2++)
		{
			for (t3 = 0; t3 < kernel_h_3; t3++)
			{
				for (t4 = 0; t4 < kernel_w_3; t4++)
				{
					W_flp_3[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l4; t1++)
	{
		b_flp_3[t1] = 0.1;
	}
	printf("Initialization 3 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l4, b_flp_3, b_fxp_3);
	FxP_Conversion_4D(nodes_l4, nodes_l3, kernel_h_3, kernel_w_3, W_flp_3, W_fxp_3);
	printf("Fixed_Point Conversion 3 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 4;
	in_layer = 4;
	printf("Reseting 3 is done \r\n");

	for( j = 0; j < nodes_l4; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l3; i++)
		{
			//WRITE_Matrix_Weight(j, i, out_Hight_2, out_Width_2, kernel_h_3, kernel_w_3, kernel_padding_3, kernel_stride_3, W_fxp_3, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_3 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_3 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l3; i++)
		{
			if(in_block_w == 1 && in_block_h == 1)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_2; p++)
			{
				for( q = 0; q < out_Hight_2; q++)
				{
					if((out_Width_2 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_2 * in_block_h);
						mm = p + (out_Width_2 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_2 - 1) && (q == out_Hight_2 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_2 - 1) && (q == out_Hight_2 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_2 * in_block_h);
						mm = p + (out_Width_2 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 12 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 1 && out_block_h == 1)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_3 - 1; n < out_Hight_2 + kernel_stride_3; n = n + kernel_stride_3)
		{
			p = 0;
			for( m = kernel_w_3 - 1; m < out_Width_2 + kernel_stride_3; m = m + kernel_stride_3)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_3 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_3 * out_block_h);
					k = p + (out_Width_3 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_3 - 1) && (q == out_Hight_3 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_3 - 1) && (q == out_Hight_3 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_3 * out_block_h);
					k = p + (out_Width_3 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_3, out_Width_3, b_fxp_3, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	}//end nodes_l4 in R21-R36 (64, 55, 55)



	// Declaration
	static float W_flp_4 [nodes_l5][nodes_l4][kernel_h_4][kernel_w_4];
	static float b_flp_4 [nodes_l5];

	static int W_fxp_4 [nodes_l5][nodes_l4][kernel_h_4][kernel_w_4];
	static int b_fxp_4 [nodes_l5];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l5; t1++)
	{
		for (t2 = 0; t2 < nodes_l4; t2++)
		{
			for (t3 = 0; t3 < kernel_h_4; t3++)
			{
				for (t4 = 0; t4 < kernel_w_4; t4++)
				{
					W_flp_4[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l5; t1++)
	{
		b_flp_4[t1] = 0.1;
	}
	printf("Initialization 4 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l5, b_flp_4, b_fxp_4);
	FxP_Conversion_4D(nodes_l5, nodes_l4, kernel_h_4, kernel_w_4, W_flp_4, W_fxp_4);
	printf("Fixed_Point Conversion 4 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 20;
	in_layer = 4;
	printf("Reseting 4 is done \r\n");

	for( j = 0; j < nodes_l5; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l4; i++)
		{
			//WRITE_Matrix_Weight(j, i, out_Hight_2, out_Width_2, kernel_h_4, kernel_w_4, kernel_padding_4, kernel_stride_4, W_fxp_4, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_4 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_4 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l4; i++)
		{
			if(in_block_w == 1 && in_block_h == 1)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_2; p++)
			{
				for( q = 0; q < out_Hight_2; q++)
				{
					if((out_Width_2 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_2 * in_block_h);
						mm = p + (out_Width_2 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_2 - 1) && (q == out_Hight_2 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_2 - 1) && (q == out_Hight_2 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_2 * in_block_h);
						mm = p + (out_Width_2 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 1 && out_block_h == 1)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_4 - 1; n < out_Hight_2 + kernel_stride_4; n = n + kernel_stride_4)
		{
			p = 0;
			for( m = kernel_w_4 - 1; m < out_Width_2 + kernel_stride_4; m = m + kernel_stride_4)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_4 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_4 * out_block_h);
					k = p + (out_Width_4 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_4 - 1) && (q == out_Hight_4 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_4 - 1) && (q == out_Hight_4 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_4 * out_block_h);
					k = p + (out_Width_4 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_4, out_Width_4, b_fxp_4, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	}//end nodes_l5 in R37-R52 (64, 55, 55)



	//fire3
	// Declaration
	static float W_flp_5 [nodes_l6][nodes_l5][kernel_h_5][kernel_w_5];
	static float b_flp_5 [nodes_l6];

	static int W_fxp_5 [nodes_l6][nodes_l5][kernel_h_5][kernel_w_5];
	static int b_fxp_5 [nodes_l6];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l6; t1++)
	{
		for (t2 = 0; t2 < nodes_l5; t2++)
		{
			for (t3 = 0; t3 < kernel_h_5; t3++)
			{
				for (t4 = 0; t4 < kernel_w_5; t4++)
				{
					W_flp_5[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l6; t1++)
	{
		b_flp_5[t1] = 0.1;
	}
	printf("Initialization 5 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l6, b_flp_5, b_fxp_5);
	FxP_Conversion_4D(nodes_l6, nodes_l5, kernel_h_5, kernel_w_5, W_flp_5, W_fxp_5);
	printf("Fixed_Point Conversion 5 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 4;
	out_arr_full = 0;
	in_layer = 32;
	printf("Reseting 5 is done \r\n");

	for( j = 0; j < nodes_l6; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l5; i++)
		{
			//WRITE_Matrix_Weight(j, i, out_Hight_4, out_Width_4, kernel_h_5, kernel_w_5, kernel_padding_5, kernel_stride_5, W_fxp_5, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_5 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_5 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l4 + nodes_l5; i++)
		{
			if(in_block_w == 1 && in_block_h == 1)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_4; p++)
			{
				for( q = 0; q < out_Hight_4; q++)
				{
					if((out_Width_4 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_4 * in_block_h);
						mm = p + (out_Width_4 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_4 - 1) && (q == out_Hight_4 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_4 - 1) && (q == out_Hight_4 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_4 * in_block_h);
						mm = p + (out_Width_4 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_5 - 1; n < out_Hight_4 + kernel_stride_5; n = n + kernel_stride_5)
		{
			p = 0;
			for( m = kernel_w_5 - 1; m < out_Width_4 + kernel_stride_5; m = m + kernel_stride_5)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_5 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_7)) < block_dim * 4)
				{
					l = q + (out_Hight_5 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_7);
					k = p + (out_Width_5 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_7);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_5 - 1) && (q == out_Hight_5 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_5 - 1) && (q == out_Hight_5 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_5 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_7);
					k = p + (out_Width_5 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_7);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_5, out_Width_5, b_fxp_5, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	}//end nodes_l6 in R17-R20 (16, 55, 55)


	// Declaration
	static float W_flp_6 [nodes_l7][nodes_l6][kernel_h_6][kernel_w_6];
	static float b_flp_6 [nodes_l7];

	static int W_fxp_6 [nodes_l7][nodes_l6][kernel_h_6][kernel_w_6];
	static int b_fxp_6 [nodes_l7];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l7; t1++)
	{
		for (t2 = 0; t2 < nodes_l6; t2++)
		{
			for (t3 = 0; t3 < kernel_h_6; t3++)
			{
				for (t4 = 0; t4 < kernel_w_6; t4++)
				{
					W_flp_6[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l7; t1++)
	{
		b_flp_6[t1] = 0.1;
	}
	printf("Initialization 6 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l7, b_flp_6, b_fxp_6);
	FxP_Conversion_4D(nodes_l7, nodes_l6, kernel_h_6, kernel_w_6, W_flp_6, W_fxp_6);
	printf("Fixed_Point Conversion 6 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 4;
	in_layer = 4;
	printf("Reseting 6 is done \r\n");

	for( j = 0; j < nodes_l7; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l6; i++)
		{
			//WRITE_Matrix_Weight(j, i, out_Hight_5, out_Width_5, kernel_h_6, kernel_w_6, kernel_padding_6, kernel_stride_6, W_fxp_6, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_6 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_6 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l6; i++)
		{
			if(in_block_w == 1 && in_block_h == 1)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_5; p++)
			{
				for( q = 0; q < out_Hight_5; q++)
				{
					if((out_Width_5 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_5 * in_block_h);
						mm = p + (out_Width_5 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_5 - 1) && (q == out_Hight_5 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_5 - 1) && (q == out_Hight_5 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_5 * in_block_h);
						mm = p + (out_Width_5 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 12 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 1 && out_block_h == 1)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_6 - 1; n < out_Hight_5 + kernel_stride_6; n = n + kernel_stride_6)
		{
			p = 0;
			for( m = kernel_w_6 - 1; m < out_Width_5 + kernel_stride_6; m = m + kernel_stride_6)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 12 + in_arr_full);
				if((out_Width_6 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_6 * out_block_h);
					k = p + (out_Width_6 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_6 - 1) && (q == out_Hight_6 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_6 - 1) && (q == out_Hight_6 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_6 * out_block_h);
					k = p + (out_Width_6 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_6, out_Width_6, b_fxp_6, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

		// Max pooling
		m_pool = 0;
		n_pool = 0;
		nn = 0;
		mm = 0;
		if(block_pool_w == 3 && block_pool_h == 3)
		{
			arr_full++;
			block_pool_w = 0;
			block_pool_h = 0;
		}
		for( y = (block_pool_h * out_Hight_6); y < ((block_pool_h + 1) * out_Hight_6) - pool_h_2 + 2 * pool_p_2 + 1; y = y + pool_s_2)
		{
			for( x = (block_pool_h * out_Width_6); x < ((block_pool_w + 1) * out_Width_6) - pool_w_2 + 2 * pool_p_2 + 1; x = x + pool_s_2)
			{
				for( y_k = y; y_k < y + pool_h_2; y_k++)
				{
					for( x_k = x; x_k < x + pool_w_2; x_k++)
					{
						signed int tmp_out = READ_REG(y_k/4, x_k/4, ((y_k*4+x_k)%4+y_k*4)%16, 17 + out_arr_full);
						if (tmp_out > max)
						{
							max = tmp_out;
						}
					}
				}
				// write max
				if((out_pool_Width_2 * (block_pool_w + 1)) < block_dim * 4)
				{
					nn = n_pool + (out_pool_Hight_2 * block_pool_h);
					mm = m_pool + (out_pool_Width_2 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 53 + arr_full, max);

					if((m_pool == out_pool_Width_2 - 1) && (n_pool == out_pool_Hight_2 - 1))
					{
						block_pool_w++;
					}
				}
				else
				{
					if((m_pool == out_pool_Width_2 - 1) && (n_pool == out_pool_Hight_2 - 1))
					{
						block_pool_h++;
						block_pool_w = 0;
					}

					nn = n_pool + (out_pool_Hight_2 * block_pool_h);
					mm = m_pool + (out_pool_Width_2 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 53 + arr_full, max);
				}


				m_pool++;
				if (m_pool == out_pool_Width_2)
				{
					m_pool = 0;
					n_pool++;
				}
				max = -100000000;

			}
		}
	}//end nodes_l7 in R21-R36 (64, 55, 55) and in R53-R56 (64, 27, 27)



	// Declaration
	static float W_flp_7 [nodes_l8][nodes_l7][kernel_h_7][kernel_w_7];
	static float b_flp_7 [nodes_l8];

	static int W_fxp_7 [nodes_l8][nodes_l7][kernel_h_7][kernel_w_7];
	static int b_fxp_7 [nodes_l8];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l8; t1++)
	{
		for (t2 = 0; t2 < nodes_l7; t2++)
		{
			for (t3 = 0; t3 < kernel_h_7; t3++)
			{
				for (t4 = 0; t4 < kernel_w_7; t4++)
				{
					W_flp_7[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l8; t1++)
	{
		b_flp_7[t1] = 0.1;
	}
	printf("Initialization 7 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l8, b_flp_7, b_fxp_7);
	FxP_Conversion_4D(nodes_l8, nodes_l7, kernel_h_7, kernel_w_7, W_flp_7, W_fxp_7);
	printf("Fixed_Point Conversion 7 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 20;
	in_layer = 4;
	printf("Reseting 7 is done \r\n");

	for( j = 0; j < nodes_l8; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l7; i++)
		{
			//WRITE_Matrix_Weight(j, i, out_Hight_5, out_Width_5, kernel_h_7, kernel_w_7, kernel_padding_7, kernel_stride_7, W_fxp_7, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_7 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_7 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l7; i++)
		{
			if(in_block_w == 1 && in_block_h == 1)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_5; p++)
			{
				for( q = 0; q < out_Hight_5; q++)
				{
					if((out_Width_5 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_5 * in_block_h);
						mm = p + (out_Width_5 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_5 - 1) && (q == out_Hight_5 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_5 - 1) && (q == out_Hight_5 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_5 * in_block_h);
						mm = p + (out_Width_5 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 1 && out_block_h == 1)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_7 - 1; n < out_Hight_5 + kernel_stride_7; n = n + kernel_stride_7)
		{
			p = 0;
			for( m = kernel_w_7 - 1; m < out_Width_5 + kernel_stride_7; m = m + kernel_stride_7)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 12 + in_arr_full);
				if((out_Width_7 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_7 * out_block_h);
					k = p + (out_Width_7 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_7 - 1) && (q == out_Hight_7 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_7 - 1) && (q == out_Hight_7 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_7 * out_block_h);
					k = p + (out_Width_7 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_7, out_Width_7, b_fxp_4, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

		// Max pooling
		m_pool = 0;
		n_pool = 0;
		nn = 0;
		mm = 0;
		if(block_pool_w == 3 && block_pool_h == 3)
		{
			arr_full++;
			block_pool_w = 0;
			block_pool_h = 0;
		}
		for( y = (block_pool_h * out_Hight_7); y < ((block_pool_h + 1) * out_Hight_7) - pool_h_2 + 2 * pool_p_2 + 1; y = y + pool_s_2)
		{
			for( x = (block_pool_h * out_Width_7); x < ((block_pool_w + 1) * out_Width_7) - pool_w_2 + 2 * pool_p_2 + 1; x = x + pool_s_2)
			{
				for( y_k = y; y_k < y + pool_h_2; y_k++)
				{
					for( x_k = x; x_k < x + pool_w_2; x_k++)
					{
						signed int tmp_out = READ_REG(y_k/4, x_k/4, ((y_k*4+x_k)%4+y_k*4)%16, 17 + out_arr_full);
						if (tmp_out > max)
						{
							max = tmp_out;
						}
					}
				}
				// write max
				if((out_pool_Width_2 * (block_pool_w + 1)) < block_dim * 4)
				{
					nn = n_pool + (out_pool_Hight_2 * block_pool_h);
					mm = m_pool + (out_pool_Width_2 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 57 + arr_full, max);

					if((m_pool == out_pool_Width_2 - 1) && (n_pool == out_pool_Hight_2 - 1))
					{
						block_pool_w++;
					}
				}
				else
				{
					if((m_pool == out_pool_Width_2 - 1) && (n_pool == out_pool_Hight_2 - 1))
					{
						block_pool_h++;
						block_pool_w = 0;
					}

					nn = n_pool + (out_pool_Hight_2 * block_pool_h);
					mm = m_pool + (out_pool_Width_2 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 57 + arr_full, max);
				}


				m_pool++;
				if (m_pool == out_pool_Width_2)
				{
					m_pool = 0;
					n_pool++;
				}
				max = -100000000;

			}
		}
	}//end nodes_l8 in R37-R52 (64, 55, 55) and in R57-R60 (64, 27, 27)

	execute(0, 17, 53, 0);
	execute(0, 18, 54, 0);
	execute(0, 19, 55, 0);
	execute(0, 20, 56, 0);
	execute(0, 21, 57, 0);
	execute(0, 22, 58, 0);
	execute(0, 23, 59, 0);
	execute(0, 24, 60, 0);

	//fire4
	// Declaration
	static float W_flp_8 [nodes_l9][nodes_l8][kernel_h_8][kernel_w_8];
	static float b_flp_8 [nodes_l9];

	static int W_fxp_8 [nodes_l9][nodes_l8][kernel_h_8][kernel_w_8];
	static int b_fxp_8 [nodes_l9];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l9; t1++)
	{
		for (t2 = 0; t2 < nodes_l8; t2++)
		{
			for (t3 = 0; t3 < kernel_h_8; t3++)
			{
				for (t4 = 0; t4 < kernel_w_8; t4++)
				{
					W_flp_8[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l9; t1++)
	{
		b_flp_8[t1] = 0.1;
	}
	printf("Initialization 8 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l9, b_flp_8, b_fxp_8);
	FxP_Conversion_4D(nodes_l9, nodes_l8, kernel_h_8, kernel_w_8, W_flp_8, W_fxp_8);
	printf("Fixed_Point Conversion 8 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 8;
	in_layer = 8;
	printf("Reseting 8 is done \r\n");

	for( j = 0; j < nodes_l9; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l8; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_7, out_Width_7, kernel_h_8, kernel_w_8, kernel_padding_8, kernel_stride_8, W_fxp_8, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_8 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_8 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l7 + nodes_l8; i++)
		{
			if(in_block_w == 3 && in_block_h == 3)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_7; p++)
			{
				for( q = 0; q < out_Hight_7; q++)
				{
					if((out_Width_7 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_7 * in_block_h);
						mm = p + (out_Width_7 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_7 - 1) && (q == out_Hight_7 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_7 - 1) && (q == out_Hight_7 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_7 * in_block_h);
						mm = p + (out_Width_7 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_8 - 1; n < out_Hight_7 + kernel_stride_8; n = n + kernel_stride_8)
		{
			p = 0;
			for( m = kernel_w_8 - 1; m < out_Width_7 + kernel_stride_8; m = m + kernel_stride_8)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_5 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_10)) < block_dim * 4)
				{
					l = q + (out_Hight_8 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_10);
					k = p + (out_Width_8 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_10);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_8 - 1) && (q == out_Hight_8 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_8 - 1) && (q == out_Hight_8 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_8 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_10);
					k = p + (out_Width_8 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_10);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_8, out_Width_8, b_fxp_8, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	}//end nodes_l9 in R25-R26 (32, 27, 27)

	execute(0, 17, 25, 0);
	execute(0, 18, 26, 0);

	// Declaration
	static float W_flp_9 [nodes_l10][nodes_l9][kernel_h_9][kernel_w_9];
	static float b_flp_9 [nodes_l10];

	static int W_fxp_9 [nodes_l10][nodes_l9][kernel_h_9][kernel_w_9];
	static int b_fxp_9 [nodes_l10];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l10; t1++)
	{
		for (t2 = 0; t2 < nodes_l9; t2++)
		{
			for (t3 = 0; t3 < kernel_h_9; t3++)
			{
				for (t4 = 0; t4 < kernel_w_9; t4++)
				{
					W_flp_9[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l10; t1++)
	{
		b_flp_9[t1] = 0.1;
	}
	printf("Initialization 9 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l10, b_flp_9, b_fxp_9);
	FxP_Conversion_4D(nodes_l10, nodes_l9, kernel_h_9, kernel_w_9, W_flp_9, W_fxp_9);
	printf("Fixed_Point Conversion 9 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 2;
	in_layer = 2;
	printf("Reseting 9 is done \r\n");

	for( j = 0; j < nodes_l10; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l9; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_8, out_Width_8, kernel_h_9, kernel_w_9, kernel_padding_9, kernel_stride_9, W_fxp_9, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_9 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_9 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l9; i++)
		{
			if(in_block_w == 3 && in_block_h == 3)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_8; p++)
			{
				for( q = 0; q < out_Hight_8; q++)
				{
					if((out_Width_8 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_8 * in_block_h);
						mm = p + (out_Width_8 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_8 - 1) && (q == out_Hight_8 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_8 - 1) && (q == out_Hight_8 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_8 * in_block_h);
						mm = p + (out_Width_8 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 3 && out_block_h == 3)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_9 - 1; n < out_Hight_8 + kernel_stride_9; n = n + kernel_stride_9)
		{
			p = 0;
			for( m = kernel_w_9 - 1; m < out_Width_8 + kernel_stride_9; m = m + kernel_stride_9)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_9 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_9 * out_block_h);
					k = p + (out_Width_9 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_9 - 1) && (q == out_Hight_9 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_9 - 1) && (q == out_Hight_9 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_9 * out_block_h);
					k = p + (out_Width_9 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_9, out_Width_9, b_fxp_9, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	}//end nodes_l10 in R19-R26 (128, 27, 27)



	// Declaration
	static float W_flp_10 [nodes_l11][nodes_l10][kernel_h_10][kernel_w_10];
	static float b_flp_10 [nodes_l11];

	static int W_fxp_10 [nodes_l11][nodes_l10][kernel_h_10][kernel_w_10];
	static int b_fxp_10 [nodes_l11];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l11; t1++)
	{
		for (t2 = 0; t2 < nodes_l10; t2++)
		{
			for (t3 = 0; t3 < kernel_h_10; t3++)
			{
				for (t4 = 0; t4 < kernel_w_10; t4++)
				{
					W_flp_10[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l11; t1++)
	{
		b_flp_10[t1] = 0.1;
	}
	printf("Initialization 10 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l11, b_flp_10, b_fxp_10);
	FxP_Conversion_4D(nodes_l11, nodes_l10, kernel_h_10, kernel_w_10, W_flp_10, W_fxp_10);
	printf("Fixed_Point Conversion 10 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 10;
	in_layer = 2;
	printf("Reseting 10 is done \r\n");

	for( j = 0; j < nodes_l11; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l10; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_8, out_Width_8, kernel_h_10, kernel_w_10, kernel_padding_10, kernel_stride_10, W_fxp_10, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_10 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_10 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l10; i++)
		{
			if(in_block_w == 3 && in_block_h == 3)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_8; p++)
			{
				for( q = 0; q < out_Hight_8; q++)
				{
					if((out_Width_8 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_8 * in_block_h);
						mm = p + (out_Width_8 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_8 - 1) && (q == out_Hight_8 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_8 - 1) && (q == out_Hight_8 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_8 * in_block_h);
						mm = p + (out_Width_8 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 3 && out_block_h == 3)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_10 - 1; n < out_Hight_8 + kernel_stride_10; n = n + kernel_stride_10)
		{
			p = 0;
			for( m = kernel_w_10 - 1; m < out_Width_8 + kernel_stride_10; m = m + kernel_stride_10)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_10 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_10 * out_block_h);
					k = p + (out_Width_10 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_10 - 1) && (q == out_Hight_10 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_10 - 1) && (q == out_Hight_10 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_10 * out_block_h);
					k = p + (out_Width_10 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_10, out_Width_10, b_fxp_10, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	}//end nodes_l11 in R27-R34 (128, 27, 27)


	//fire5
	// Declaration
	static float W_flp_11 [nodes_l12][nodes_l11][kernel_h_11][kernel_w_11];
	static float b_flp_11 [nodes_l12];

	static int W_fxp_11 [nodes_l12][nodes_l11][kernel_h_11][kernel_w_11];
	static int b_fxp_11 [nodes_l12];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l12; t1++)
	{
		for (t2 = 0; t2 < nodes_l11; t2++)
		{
			for (t3 = 0; t3 < kernel_h_11; t3++)
			{
				for (t4 = 0; t4 < kernel_w_11; t4++)
				{
					W_flp_11[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l12; t1++)
	{
		b_flp_11[t1] = 0.1;
	}
	printf("Initialization 11 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l12, b_flp_11, b_fxp_11);
	FxP_Conversion_4D(nodes_l12, nodes_l11, kernel_h_11, kernel_w_11, W_flp_11, W_fxp_11);
	printf("Fixed_Point Conversion 11 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 2;
	out_arr_full = 0;
	in_layer = 16;
	printf("Reseting 11 is done \r\n");

	for( j = 0; j < nodes_l12; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l11; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_10, out_Width_10, kernel_h_11, kernel_w_11, kernel_padding_11, kernel_stride_11, W_fxp_11, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_11 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_11 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l10 + nodes_l11; i++)
		{
			if(block_pool_w == 3 && block_pool_h == 3)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_10; p++)
			{
				for( q = 0; q < out_Hight_10; q++)
				{
					if((out_Width_10 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_10 * in_block_h);
						mm = p + (out_Width_10 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_10 - 1) && (q == out_Hight_10 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_10 - 1) && (q == out_Hight_10 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_10 * in_block_h);
						mm = p + (out_Width_10 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_11 - 1; n < out_Hight_10 + kernel_stride_11; n = n + kernel_stride_11)
		{
			p = 0;
			for( m = kernel_w_11 - 1; m < out_Width_10 + kernel_stride_11; m = m + kernel_stride_11)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_11 * out_block_w + (2 * out_block_w + 1) * (kernel_padding_13))) < block_dim * 4)
				{
					l = q + (out_Hight_11 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_13);
					k = p + (out_Width_11 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_13);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_11 - 1) && (q == out_Hight_11 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_11 - 1) && (q == out_Hight_11 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_11 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_13);
					k = p + (out_Width_11 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_13);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_11, out_Width_11, b_fxp_11, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	}//end nodes_l12 in R17-R18 (32, 27, 27)


	// Declaration
	static float W_flp_12 [nodes_l13][nodes_l12][kernel_h_12][kernel_w_12];
	static float b_flp_12 [nodes_l13];

	static int W_fxp_12 [nodes_l13][nodes_l12][kernel_h_12][kernel_w_12];
	static int b_fxp_12 [nodes_l13];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l13; t1++)
	{
		for (t2 = 0; t2 < nodes_l12; t2++)
		{
			for (t3 = 0; t3 < kernel_h_12; t3++)
			{
				for (t4 = 0; t4 < kernel_w_12; t4++)
				{
					W_flp_12[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l13; t1++)
	{
		b_flp_12[t1] = 0.1;
	}
	printf("Initialization 12 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l13, b_flp_12, b_fxp_12);
	FxP_Conversion_4D(nodes_l13, nodes_l12, kernel_h_12, kernel_w_12, W_flp_12, W_fxp_12);
	printf("Fixed_Point Conversion 9 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 2;
	in_layer = 2;
	printf("Reseting 12 is done \r\n");

	for( j = 0; j < nodes_l13; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l12; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_11, out_Width_11, kernel_h_12, kernel_w_12, kernel_padding_12, kernel_stride_12, W_fxp_12, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_12 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_12 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l12; i++)
		{
			if(in_block_w == 3 && in_block_h == 3)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_11; p++)
			{
				for( q = 0; q < out_Hight_11; q++)
				{
					if((out_Width_8 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_11 * in_block_h);
						mm = p + (out_Width_11 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_11 - 1) && (q == out_Hight_11 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_11 - 1) && (q == out_Hight_11 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_11 * in_block_h);
						mm = p + (out_Width_11 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_12 - 1; n < out_Hight_11 + kernel_stride_12; n = n + kernel_stride_12)
		{
			p = 0;
			for( m = kernel_w_12 - 1; m < out_Width_11 + kernel_stride_12; m = m + kernel_stride_12)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_12 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_12 * out_block_h);
					k = p + (out_Width_12 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_12 - 1) && (q == out_Hight_12 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_12 - 1) && (q == out_Hight_12 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_12 * out_block_h);
					k = p + (out_Width_12 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_12, out_Width_12, b_fxp_12, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

		// Max pooling
		m_pool = 0;
		n_pool = 0;
		nn = 0;
		mm = 0;
		if(block_pool_w == 7 && block_pool_h == 7)
		{
			arr_full++;
			block_pool_w = 0;
			block_pool_h = 0;
		}
		for( y = (block_pool_h * out_Hight_12); y < ((block_pool_h + 1) * out_Hight_12) - pool_h_3 + 2 * pool_p_3 + 1; y = y + pool_s_3)
		{
			for( x = (block_pool_h * out_Width_12); x < ((block_pool_w + 1) * out_Width_12) - pool_w_3 + 2 * pool_p_3 + 1; x = x + pool_s_3)
			{
				for( y_k = y; y_k < y + pool_h_3; y_k++)
				{
					for( x_k = x; x_k < x + pool_w_3; x_k++)
					{
						signed int tmp_out = READ_REG(y_k/4, x_k/4, ((y_k*4+x_k)%4+y_k*4)%16, 17 + out_arr_full);
						if (tmp_out > max)
						{
							max = tmp_out;
						}
					}
				}
				// write max
				if((out_pool_Width_3 * (block_pool_w + 1)) < block_dim * 4)
				{
					nn = n_pool + (out_pool_Hight_3 * block_pool_h);
					mm = m_pool + (out_pool_Width_3 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 35 + arr_full, max);

					if((m_pool == out_pool_Width_3 - 1) && (n_pool == out_pool_Hight_3 - 1))
					{
						block_pool_w++;
					}
				}
				else
				{
					if((m_pool == out_pool_Width_3 - 1) && (n_pool == out_pool_Hight_3 - 1))
					{
						block_pool_h++;
						block_pool_w = 0;
					}

					nn = n_pool + (out_pool_Hight_3 * block_pool_h);
					mm = m_pool + (out_pool_Width_3 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 35 + arr_full, max);
				}


				m_pool++;
				if (m_pool == out_pool_Width_3)
				{
					m_pool = 0;
					n_pool++;
				}
				max = -100000000;

			}
		}

	} //end nodes_l13 in R19-R26 (128, 27, 27) and in R35-R36 (128, 13, 13)


	// Declaration
	static float W_flp_13 [nodes_l14][nodes_l13][kernel_h_13][kernel_w_13];
	static float b_flp_13 [nodes_l14];

	static int W_fxp_13 [nodes_l14][nodes_l13][kernel_h_13][kernel_w_13];
	static int b_fxp_13 [nodes_l14];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l14; t1++)
	{
		for (t2 = 0; t2 < nodes_l13; t2++)
		{
			for (t3 = 0; t3 < kernel_h_13; t3++)
			{
				for (t4 = 0; t4 < kernel_w_13; t4++)
				{
					W_flp_13[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l14; t1++)
	{
		b_flp_13[t1] = 0.1;
	}
	printf("Initialization 13 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l14, b_flp_13, b_fxp_13);
	FxP_Conversion_4D(nodes_l14, nodes_l13, kernel_h_13, kernel_w_13, W_flp_13, W_fxp_13);
	printf("Fixed_Point Conversion 13 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 10;
	in_layer = 2;
	printf("Reseting 13 is done \r\n");

	for( j = 0; j < nodes_l14; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l13; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_11, out_Width_11, kernel_h_13, kernel_w_13, kernel_padding_13, kernel_stride_13, W_fxp_13, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_13 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_13 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l13; i++)
		{
			if(in_block_w == 3 && in_block_h == 3)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_11; p++)
			{
				for( q = 0; q < out_Hight_11; q++)
				{
					if((out_Width_11 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_11 * in_block_h);
						mm = p + (out_Width_11 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_11 - 1) && (q == out_Hight_11 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_11 - 1) && (q == out_Hight_11 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_11 * in_block_h);
						mm = p + (out_Width_11 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_13 - 1; n < out_Hight_11 + kernel_stride_13; n = n + kernel_stride_13)
		{
			p = 0;
			for( m = kernel_w_13 - 1; m < out_Width_11 + kernel_stride_13; m = m + kernel_stride_13)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_13 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_13 * out_block_h);
					k = p + (out_Width_13 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_13 - 1) && (q == out_Hight_13 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_13 - 1) && (q == out_Hight_13 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_13 * out_block_h);
					k = p + (out_Width_13 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_13, out_Width_13, b_fxp_13, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

		// Max pooling
		m_pool = 0;
		n_pool = 0;
		nn = 0;
		mm = 0;
		if(block_pool_w == 7 && block_pool_h == 7)
		{
			arr_full++;
			block_pool_w = 0;
			block_pool_h = 0;
		}
		for( y = (block_pool_h * out_Hight_13); y < ((block_pool_h + 1) * out_Hight_13) - pool_h_3 + 2 * pool_p_3 + 1; y = y + pool_s_3)
		{
			for( x = (block_pool_h * out_Width_13); x < ((block_pool_w + 1) * out_Width_13) - pool_w_3 + 2 * pool_p_3 + 1; x = x + pool_s_3)
			{
				for( y_k = y; y_k < y + pool_h_3; y_k++)
				{
					for( x_k = x; x_k < x + pool_w_3; x_k++)
					{
						signed int tmp_out = READ_REG(y_k/4, x_k/4, ((y_k*4+x_k)%4+y_k*4)%16, 17 + out_arr_full);
						if (tmp_out > max)
						{
							max = tmp_out;
						}
					}
				}
				// write max
				if((out_pool_Width_3 * (block_pool_w + 1)) < block_dim * 4)
				{
					nn = n_pool + (out_pool_Hight_3 * block_pool_h);
					mm = m_pool + (out_pool_Width_3 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 37 + arr_full, max);

					if((m_pool == out_pool_Width_3 - 1) && (n_pool == out_pool_Hight_3 - 1))
					{
						block_pool_w++;
					}
				}
				else
				{
					if((m_pool == out_pool_Width_3 - 1) && (n_pool == out_pool_Hight_3 - 1))
					{
						block_pool_h++;
						block_pool_w = 0;
					}

					nn = n_pool + (out_pool_Hight_3 * block_pool_h);
					mm = m_pool + (out_pool_Width_3 * block_pool_w);

					WRITE_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 37 + arr_full, max);
				}


				m_pool++;
				if (m_pool == out_pool_Width_3)
				{
					m_pool = 0;
					n_pool++;
				}
				max = -100000000;

			}
		}

	} //end nodes_l14 in R27-R34 (128, 27, 27) and in R37-R38 (128, 13, 13)

	execute(0, 17, 35, 0);
	execute(0, 18, 36, 0);
	execute(0, 19, 37, 0);
	execute(0, 20, 38, 0);

	//fire6
	// Declaration
	static float W_flp_14 [nodes_l15][nodes_l14][kernel_h_14][kernel_w_14];
	static float b_flp_14 [nodes_l15];

	static int W_fxp_14 [nodes_l15][nodes_l14][kernel_h_14][kernel_w_14];
	static int b_fxp_14 [nodes_l15];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l15; t1++)
	{
		for (t2 = 0; t2 < nodes_l14; t2++)
		{
			for (t3 = 0; t3 < kernel_h_14; t3++)
			{
				for (t4 = 0; t4 < kernel_w_14; t4++)
				{
					W_flp_14[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l15; t1++)
	{
		b_flp_14[t1] = 0.1;
	}
	printf("Initialization 14 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l15, b_flp_14, b_fxp_14);
	FxP_Conversion_4D(nodes_l15, nodes_l14, kernel_h_14, kernel_w_14, W_flp_14, W_fxp_14);
	printf("Fixed_Point Conversion 14 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 4;
	in_layer = 4;
	printf("Reseting 14 is done \r\n");

	for( j = 0; j < nodes_l15; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l14; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_13, out_Width_13, kernel_h_14, kernel_w_14, kernel_padding_14, kernel_stride_14, W_fxp_14, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_14 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_14 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l14; i++)
		{
			if(in_block_w == 7 && in_block_h == 7)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_13; p++)
			{
				for( q = 0; q < out_Hight_13; q++)
				{
					if((out_Width_13 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_13 * in_block_h);
						mm = p + (out_Width_13 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_13 - 1) && (q == out_Hight_13 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_13 - 1) && (q == out_Hight_13 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_13 * in_block_h);
						mm = p + (out_Width_13 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_14 - 1; n < out_Hight_13 + kernel_stride_14; n = n + kernel_stride_14)
		{
			p = 0;
			for( m = kernel_w_14 - 1; m < out_Width_13 + kernel_stride_14; m = m + kernel_stride_14)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_14 * out_block_w) + (2 * out_block_h + 1) * (kernel_padding_16)) < block_dim * 4)
				{
					l = q + (out_Hight_14 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_16);
					k = p + (out_Width_14 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_16);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_14 - 1) && (q == out_Hight_14 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_14 - 1) && (q == out_Hight_14 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_14 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_16);
					k = p + (out_Width_14 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_16);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_14, out_Width_14, b_fxp_11, 14); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	} //end nodes_l15 in R21 (48, 13, 13)

	execute(0, 17, 21, 0);

	// Declaration
	static float W_flp_15 [nodes_l16][nodes_l15][kernel_h_15][kernel_w_15];
	static float b_flp_15 [nodes_l16];

	static int W_fxp_15 [nodes_l16][nodes_l15][kernel_h_15][kernel_w_15];
	static int b_fxp_15 [nodes_l16];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l16; t1++)
	{
		for (t2 = 0; t2 < nodes_l15; t2++)
		{
			for (t3 = 0; t3 < kernel_h_15; t3++)
			{
				for (t4 = 0; t4 < kernel_w_15; t4++)
				{
					W_flp_15[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l16; t1++)
	{
		b_flp_15[t1] = 0.1;
	}
	printf("Initialization 15 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l16, b_flp_15, b_fxp_15);
	FxP_Conversion_4D(nodes_l16, nodes_l15, kernel_h_15, kernel_w_15, W_flp_15, W_fxp_15);
	printf("Fixed_Point Conversion 15 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 1;
	in_layer = 1;
	printf("Reseting 15 is done \r\n");

	for( j = 0; j < nodes_l16; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l15; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_14, out_Width_14, kernel_h_15, kernel_w_15, kernel_padding_15, kernel_stride_15, W_fxp_15, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_15 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_15 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l15; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_14; p++)
			{
				for( q = 0; q < out_Hight_14; q++)
				{
					if((out_Width_14 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_14 * in_block_h);
						mm = p + (out_Width_14 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_14 - 1) && (q == out_Hight_14 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_14 - 1) && (q == out_Hight_14 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_14 * in_block_h);
						mm = p + (out_Width_14 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_15 - 1; n < out_Hight_14 + kernel_stride_15; n = n + kernel_stride_15)
		{
			p = 0;
			for( m = kernel_w_15 - 1; m < out_Width_14 + kernel_stride_15; m = m + kernel_stride_15)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_15 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_15 * out_block_h);
					k = p + (out_Width_15 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_15 - 1) && (q == out_Hight_15 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_15 - 1) && (q == out_Hight_15 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_15 * out_block_h);
					k = p + (out_Width_15 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_15, out_Width_15, b_fxp_15, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	} //end nodes_l16 in R18-R20  (192, 13, 13)



	// Declaration
	static float W_flp_16 [nodes_l17][nodes_l16][kernel_h_16][kernel_w_16];
	static float b_flp_16 [nodes_l17];

	static int W_fxp_16 [nodes_l17][nodes_l16][kernel_h_16][kernel_w_16];
	static int b_fxp_16 [nodes_l17];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l17; t1++)
	{
		for (t2 = 0; t2 < nodes_l16; t2++)
		{
			for (t3 = 0; t3 < kernel_h_16; t3++)
			{
				for (t4 = 0; t4 < kernel_w_16; t4++)
				{
					W_flp_16[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l17; t1++)
	{
		b_flp_16[t1] = 0.1;
	}
	printf("Initialization 16 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l17, b_flp_16, b_fxp_16);
	FxP_Conversion_4D(nodes_l17, nodes_l16, kernel_h_16, kernel_w_16, W_flp_16, W_fxp_16);
	printf("Fixed_Point Conversion 16 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 4;
	in_layer = 1;
	printf("Reseting 16 is done \r\n");

	for( j = 0; j < nodes_l17; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l16; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_14, out_Width_14, kernel_h_16, kernel_w_16, kernel_padding_16, kernel_stride_16, W_fxp_16, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_16 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_16 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l16; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_14; p++)
			{
				for( q = 0; q < out_Hight_14; q++)
				{
					if((out_Width_14 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_14 * in_block_h);
						mm = p + (out_Width_14 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_14 - 1) && (q == out_Hight_14 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_14 - 1) && (q == out_Hight_14 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_14 * in_block_h);
						mm = p + (out_Width_14 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_16 - 1; n < out_Hight_14 + kernel_stride_16; n = n + kernel_stride_16)
		{
			p = 0;
			for( m = kernel_w_16 - 1; m < out_Width_14 + kernel_stride_16; m = m + kernel_stride_16)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_16 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_16 * out_block_h);
					k = p + (out_Width_16 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_16 - 1) && (q == out_Hight_16 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_16 - 1) && (q == out_Hight_16 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_16 * out_block_h);
					k = p + (out_Width_16 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_16, out_Width_16, b_fxp_16, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	} //end nodes_l17 in R21-R23  (192, 13, 13)


	//fire7
	// Declaration
	static float W_flp_17 [nodes_l18][nodes_l17][kernel_h_17][kernel_w_17];
	static float b_flp_17 [nodes_l18];

	static int W_fxp_17 [nodes_l18][nodes_l17][kernel_h_17][kernel_w_17];
	static int b_fxp_17 [nodes_l18];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l18; t1++)
	{
		for (t2 = 0; t2 < nodes_l17; t2++)
		{
			for (t3 = 0; t3 < kernel_h_17; t3++)
			{
				for (t4 = 0; t4 < kernel_w_17; t4++)
				{
					W_flp_17[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l18; t1++)
	{
		b_flp_17[t1] = 0.1;
	}
	printf("Initialization 17 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l18, b_flp_17, b_fxp_17);
	FxP_Conversion_4D(nodes_l18, nodes_l17, kernel_h_17, kernel_w_17, W_flp_17, W_fxp_17);
	printf("Fixed_Point Conversion 17 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 1;
	out_arr_full = 0;
	in_layer = 6;
	printf("Reseting 17 is done \r\n");

	for( j = 0; j < nodes_l18; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l17; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_16, out_Width_16, kernel_h_17, kernel_w_17, kernel_padding_17, kernel_stride_17, W_fxp_17, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_17 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_17 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l16 + nodes_l17; i++)
		{
			if(in_block_w == 7 && in_block_h == 7)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_16; p++)
			{
				for( q = 0; q < out_Hight_16; q++)
				{
					if((out_Width_16 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_16 * in_block_h);
						mm = p + (out_Width_16 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_16 - 1) && (q == out_Hight_16 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_16 - 1) && (q == out_Hight_16 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_16 * in_block_h);
						mm = p + (out_Width_16 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_17 - 1; n < out_Hight_16 + kernel_stride_17; n = n + kernel_stride_17)
		{
			p = 0;
			for( m = kernel_w_17 - 1; m < out_Width_16 + kernel_stride_17; m = m + kernel_stride_17)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_17 * out_block_w) + (2 * out_block_h + 1) * (kernel_padding_19)) < block_dim * 4)
				{
					l = q + (out_Hight_17 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_19);
					k = p + (out_Width_17 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_19);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_17 - 1) && (q == out_Hight_17 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_17 - 1) && (q == out_Hight_17 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_17 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_19);
					k = p + (out_Width_17 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_19);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_17, out_Width_17, b_fxp_17, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	} //end nodes_l18 in R17 (48, 13, 13)


	// Declaration
	static float W_flp_18 [nodes_l19][nodes_l18][kernel_h_18][kernel_w_18];
	static float b_flp_18 [nodes_l19];

	static int W_fxp_18 [nodes_l19][nodes_l18][kernel_h_18][kernel_w_18];
	static int b_fxp_18 [nodes_l19];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l19; t1++)
	{
		for (t2 = 0; t2 < nodes_l18; t2++)
		{
			for (t3 = 0; t3 < kernel_h_18; t3++)
			{
				for (t4 = 0; t4 < kernel_w_18; t4++)
				{
					W_flp_18[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l19; t1++)
	{
		b_flp_18[t1] = 0.1;
	}
	printf("Initialization 18 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l19, b_flp_18, b_fxp_18);
	FxP_Conversion_4D(nodes_l19, nodes_l18, kernel_h_18, kernel_w_18, W_flp_18, W_fxp_18);
	printf("Fixed_Point Conversion 18 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 1;
	in_layer = 1;
	printf("Reseting 18 is done \r\n");

	for( j = 0; j < nodes_l19; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l18; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_17, out_Width_17, kernel_h_18, kernel_w_18, kernel_padding_18, kernel_stride_18, W_fxp_18, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_18 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_18 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l18; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_17; p++)
			{
				for( q = 0; q < out_Hight_17; q++)
				{
					if((out_Width_17 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_17 * in_block_h);
						mm = p + (out_Width_17 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_17 - 1) && (q == out_Hight_17 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_17 - 1) && (q == out_Hight_17 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_17 * in_block_h);
						mm = p + (out_Width_17 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_18 - 1; n < out_Hight_17 + kernel_stride_18; n = n + kernel_stride_18)
		{
			p = 0;
			for( m = kernel_w_18 - 1; m < out_Width_17 + kernel_stride_18; m = m + kernel_stride_18)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_18 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_18 * out_block_h);
					k = p + (out_Width_18 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_18 - 1) && (q == out_Hight_18 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_18 - 1) && (q == out_Hight_18 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_18 * out_block_h);
					k = p + (out_Width_18 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_18, out_Width_18, b_fxp_18, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	} //end nodes_l19 in R18-R20  (192, 13, 13)



	// Declaration
	static float W_flp_19 [nodes_l20][nodes_l19][kernel_h_19][kernel_w_19];
	static float b_flp_19 [nodes_l20];

	static int W_fxp_19 [nodes_l20][nodes_l19][kernel_h_19][kernel_w_19];
	static int b_fxp_19 [nodes_l20];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l20; t1++)
	{
		for (t2 = 0; t2 < nodes_l19; t2++)
		{
			for (t3 = 0; t3 < kernel_h_19; t3++)
			{
				for (t4 = 0; t4 < kernel_w_19; t4++)
				{
					W_flp_19[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l20; t1++)
	{
		b_flp_19[t1] = 0.1;
	}
	printf("Initialization 19 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l20, b_flp_19, b_fxp_19);
	FxP_Conversion_4D(nodes_l20, nodes_l19, kernel_h_19, kernel_w_19, W_flp_19, W_fxp_19);
	printf("Fixed_Point Conversion 19 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 4;
	in_layer = 1;
	printf("Reseting 19 is done \r\n");

	for( j = 0; j < nodes_l20; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l19; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_17, out_Width_17, kernel_h_19, kernel_w_19, kernel_padding_19, kernel_stride_19, W_fxp_19, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_19 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_19 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l19; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_17; p++)
			{
				for( q = 0; q < out_Hight_17; q++)
				{
					if((out_Width_17 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_17 * in_block_h);
						mm = p + (out_Width_17 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_17 - 1) && (q == out_Hight_17 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_17 - 1) && (q == out_Hight_17 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_17 * in_block_h);
						mm = p + (out_Width_17 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_19 - 1; n < out_Hight_17 + kernel_stride_19; n = n + kernel_stride_19)
		{
			p = 0;
			for( m = kernel_w_19 - 1; m < out_Width_17 + kernel_stride_19; m = m + kernel_stride_19)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_19 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_19 * out_block_h);
					k = p + (out_Width_19 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_19 - 1) && (q == out_Hight_19 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_19 - 1) && (q == out_Hight_19 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_19 * out_block_h);
					k = p + (out_Width_19 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_19, out_Width_19, b_fxp_19, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	} //end nodes_l20 in R21-R23  (192, 13, 13)


	//fire8
	// Declaration
	static float W_flp_20 [nodes_l21][nodes_l20][kernel_h_20][kernel_w_20];
	static float b_flp_20 [nodes_l21];

	static int W_fxp_20 [nodes_l21][nodes_l20][kernel_h_20][kernel_w_20];
	static int b_fxp_20 [nodes_l21];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l21; t1++)
	{
		for (t2 = 0; t2 < nodes_l20; t2++)
		{
			for (t3 = 0; t3 < kernel_h_20; t3++)
			{
				for (t4 = 0; t4 < kernel_w_20; t4++)
				{
					W_flp_20[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l21; t1++)
	{
		b_flp_20[t1] = 0.1;
	}
	printf("Initialization 20 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l21, b_flp_20, b_fxp_20);
	FxP_Conversion_4D(nodes_l21, nodes_l20, kernel_h_20, kernel_w_20, W_flp_20, W_fxp_20);
	printf("Fixed_Point Conversion 20 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 1;
	out_arr_full = 0;
	in_layer = 6;
	printf("Reseting 20 is done \r\n");

	for( j = 0; j < nodes_l21; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l20; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_19, out_Width_19, kernel_h_20, kernel_w_20, kernel_padding_20, kernel_stride_20, W_fxp_20, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_20 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_20 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l19 + nodes_l20; i++)
		{
			if(in_block_w == 7 && in_block_h == 7)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_19; p++)
			{
				for( q = 0; q < out_Hight_19; q++)
				{
					if((out_Width_19 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_19 * in_block_h);
						mm = p + (out_Width_19 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_19 - 1) && (q == out_Hight_19 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_19 - 1) && (q == out_Hight_19 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_19 * in_block_h);
						mm = p + (out_Width_19 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_20 - 1; n < out_Hight_19 + kernel_stride_20; n = n + kernel_stride_20)
		{
			p = 0;
			for( m = kernel_w_20 - 1; m < out_Width_19 + kernel_stride_20; m = m + kernel_stride_20)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_20 * out_block_w) + (2 * out_block_h + 1) * (kernel_padding_22)) < block_dim * 4)
				{
					l = q + (out_Hight_20 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_22);
					k = p + (out_Width_20 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_22);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_20 - 1) && (q == out_Hight_20 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_20 - 1) && (q == out_Hight_20 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_20 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_22);
					k = p + (out_Width_20 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_22);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_20, out_Width_20, b_fxp_20, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	} //end nodes_l21 in R17 (64, 13, 13)


	// Declaration
	static float W_flp_21 [nodes_l22][nodes_l21][kernel_h_21][kernel_w_21];
	static float b_flp_21 [nodes_l22];

	static int W_fxp_21 [nodes_l22][nodes_l21][kernel_h_21][kernel_w_21];
	static int b_fxp_21 [nodes_l22];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l22; t1++)
	{
		for (t2 = 0; t2 < nodes_l21; t2++)
		{
			for (t3 = 0; t3 < kernel_h_21; t3++)
			{
				for (t4 = 0; t4 < kernel_w_21; t4++)
				{
					W_flp_21[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l22; t1++)
	{
		b_flp_21[t1] = 0.1;
	}
	printf("Initialization 21 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l22, b_flp_21, b_fxp_21);
	FxP_Conversion_4D(nodes_l22, nodes_l21, kernel_h_21, kernel_w_21, W_flp_21, W_fxp_21);
	printf("Fixed_Point Conversion 21 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 1;
	in_layer = 1;
	printf("Reseting 21 is done \r\n");

	for( j = 0; j < nodes_l22; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l21; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_20, out_Width_20, kernel_h_21, kernel_w_21, kernel_padding_21, kernel_stride_21, W_fxp_21, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_21 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_21 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l21; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_20; p++)
			{
				for( q = 0; q < out_Hight_20; q++)
				{
					if((out_Width_20 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_20 * in_block_h);
						mm = p + (out_Width_20 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_20 - 1) && (q == out_Hight_20 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_20 - 1) && (q == out_Hight_20 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_20 * in_block_h);
						mm = p + (out_Width_20 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 7 && out_block_h == 7)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_21 - 1; n < out_Hight_20 + kernel_stride_21; n = n + kernel_stride_21)
		{
			p = 0;
			for( m = kernel_w_21 - 1; m < out_Width_20 + kernel_stride_21; m = m + kernel_stride_21)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_21 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_21 * out_block_h);
					k = p + (out_Width_21 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_21 - 1) && (q == out_Hight_21 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_21 - 1) && (q == out_Hight_21 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_21 * out_block_h);
					k = p + (out_Width_21 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_21, out_Width_21, b_fxp_21, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	} //end nodes_l22 in R18-R21  (256, 13, 13)



	// Declaration
	static float W_flp_22 [nodes_l23][nodes_l22][kernel_h_22][kernel_w_22];
	static float b_flp_22 [nodes_l23];

	static int W_fxp_22 [nodes_l23][nodes_l22][kernel_h_22][kernel_w_22];
	static int b_fxp_22 [nodes_l23];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l23; t1++)
	{
		for (t2 = 0; t2 < nodes_l22; t2++)
		{
			for (t3 = 0; t3 < kernel_h_22; t3++)
			{
				for (t4 = 0; t4 < kernel_w_22; t4++)
				{
					W_flp_22[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l23; t1++)
	{
		b_flp_22[t1] = 0.1;
	}
	printf("Initialization 22 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l23, b_flp_22, b_fxp_22);
	FxP_Conversion_4D(nodes_l23, nodes_l22, kernel_h_22, kernel_w_22, W_flp_22, W_fxp_22);
	printf("Fixed_Point Conversion 22 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 5;
	in_layer = 1;
	printf("Reseting 22 is done \r\n");

	for( j = 0; j < nodes_l23; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l22; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_20, out_Width_20, kernel_h_22, kernel_w_22, kernel_padding_22, kernel_stride_22, W_fxp_22, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_22 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_22 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l22; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_20; p++)
			{
				for( q = 0; q < out_Hight_20; q++)
				{
					if((out_Width_20 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_20 * in_block_h);
						mm = p + (out_Width_20 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_20 - 1) && (q == out_Hight_20 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_20 - 1) && (q == out_Hight_20 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_20 * in_block_h);
						mm = p + (out_Width_20 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 7 && out_block_h == 7)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_22 - 1; n < out_Hight_20 + kernel_stride_22; n = n + kernel_stride_22)
		{
			p = 0;
			for( m = kernel_w_22 - 1; m < out_Width_20 + kernel_stride_22; m = m + kernel_stride_22)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_22 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_22 * out_block_h);
					k = p + (out_Width_22 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_22 - 1) && (q == out_Hight_22 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_22 - 1) && (q == out_Hight_22 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_22 * out_block_h);
					k = p + (out_Width_22 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_22, out_Width_22, b_fxp_22, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;


	} //end nodes_l23 in R22-R25 (256, 13, 13)


	//fire9
	// Declaration
	static float W_flp_23 [nodes_l24][nodes_l23][kernel_h_23][kernel_w_23];
	static float b_flp_23 [nodes_l24];

	static int W_fxp_23 [nodes_l24][nodes_l23][kernel_h_23][kernel_w_23];
	static int b_fxp_23 [nodes_l24];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l24; t1++)
	{
		for (t2 = 0; t2 < nodes_l23; t2++)
		{
			for (t3 = 0; t3 < kernel_h_23; t3++)
			{
				for (t4 = 0; t4 < kernel_w_23; t4++)
				{
					W_flp_23[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l24; t1++)
	{
		b_flp_23[t1] = 0.1;
	}
	printf("Initialization 23 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l24, b_flp_23, b_fxp_23);
	FxP_Conversion_4D(nodes_l24, nodes_l23, kernel_h_23, kernel_w_23, W_flp_23, W_fxp_23);
	printf("Fixed_Point Conversion 23 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 1;
	out_arr_full = 0;
	in_layer = 8;
	printf("Reseting 23 is done \r\n");

	for( j = 0; j < nodes_l24; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l23; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_22, out_Width_22, kernel_h_23, kernel_w_23, kernel_padding_23, kernel_stride_23, W_fxp_23, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_23 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_23 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l22 + nodes_l23; i++)
		{
			if(in_block_w == 7 && in_block_h == 7)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_22; p++)
			{
				for( q = 0; q < out_Hight_22; q++)
				{
					if((out_Width_22 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_22 * in_block_h);
						mm = p + (out_Width_22 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_22 - 1) && (q == out_Hight_22 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_22 - 1) && (q == out_Hight_22 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_22 * in_block_h);
						mm = p + (out_Width_22 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_23 - 1; n < out_Hight_22 + kernel_stride_23; n = n + kernel_stride_23)
		{
			p = 0;
			for( m = kernel_w_23 - 1; m < out_Width_22 + kernel_stride_23; m = m + kernel_stride_23)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if(((out_Width_23 * out_block_w) + (2 * out_block_h + 1) * (kernel_padding_25)) < block_dim * 4)
				{
					l = q + (out_Hight_23 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_25);
					k = p + (out_Width_23 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_25);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_23 - 1) && (q == out_Hight_23 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_23 - 1) && (q == out_Hight_23 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_23 * out_block_h) + (2 * out_block_h + 1) * (kernel_padding_25);
					k = p + (out_Width_23 * out_block_w) + (2 * out_block_w + 1) * (kernel_padding_25);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_23, out_Width_23, b_fxp_23, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	} //end nodes_l21 in R17 (64, 13, 13)


	// Declaration
	static float W_flp_24 [nodes_l25][nodes_l24][kernel_h_24][kernel_w_24];
	static float b_flp_24 [nodes_l25];

	static int W_fxp_24 [nodes_l25][nodes_l24][kernel_h_24][kernel_w_24];
	static int b_fxp_24 [nodes_l25];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l25; t1++)
	{
		for (t2 = 0; t2 < nodes_l24; t2++)
		{
			for (t3 = 0; t3 < kernel_h_24; t3++)
			{
				for (t4 = 0; t4 < kernel_w_24; t4++)
				{
					W_flp_24[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l25; t1++)
	{
		b_flp_24[t1] = 0.1;
	}
	printf("Initialization 24 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l25, b_flp_24, b_fxp_24);
	FxP_Conversion_4D(nodes_l25, nodes_l24, kernel_h_24, kernel_w_24, W_flp_24, W_fxp_24);
	printf("Fixed_Point Conversion 24 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 1;
	in_layer = 1;
	printf("Reseting 24 is done \r\n");

	for( j = 0; j < nodes_l25; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l24; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_23, out_Width_23, kernel_h_24, kernel_w_24, kernel_padding_24, kernel_stride_24, W_fxp_24, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i + in_arr_full, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_24 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_24 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i + in_arr_full, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l24; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_23; p++)
			{
				for( q = 0; q < out_Hight_23; q++)
				{
					if((out_Width_23 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_23 * in_block_h);
						mm = p + (out_Width_23 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_23 - 1) && (q == out_Hight_23 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_23 - 1) && (q == out_Hight_23 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_23 * in_block_h);
						mm = p + (out_Width_23 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 7 && out_block_h == 7)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_24 - 1; n < out_Hight_23 + kernel_stride_24; n = n + kernel_stride_24)
		{
			p = 0;
			for( m = kernel_w_24 - 1; m < out_Width_23 + kernel_stride_24; m = m + kernel_stride_24)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_24 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_24 * out_block_h);
					k = p + (out_Width_24 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_24 - 1) && (q == out_Hight_24 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_24 - 1) && (q == out_Hight_24 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_24 * out_block_h);
					k = p + (out_Width_24 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_24, out_Width_24, b_fxp_24, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;

	} //end nodes_l25 in R18-R21  (256, 13, 13)



	// Declaration
	static float W_flp_25 [nodes_l26][nodes_l25][kernel_h_25][kernel_w_25];
	static float b_flp_25 [nodes_l26];

	static int W_fxp_25 [nodes_l26][nodes_l25][kernel_h_25][kernel_w_25];
	static int b_fxp_25 [nodes_l26];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l26; t1++)
	{
		for (t2 = 0; t2 < nodes_l25; t2++)
		{
			for (t3 = 0; t3 < kernel_h_25; t3++)
			{
				for (t4 = 0; t4 < kernel_w_25; t4++)
				{
					W_flp_25[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l26; t1++)
	{
		b_flp_25[t1] = 0.1;
	}
	printf("Initialization 25 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l26, b_flp_25, b_fxp_25);
	FxP_Conversion_4D(nodes_l26, nodes_l25, kernel_h_25, kernel_w_25, W_flp_25, W_fxp_25);
	printf("Fixed_Point Conversion 25 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 0;
	out_arr_full = 5;
	in_layer = 1;
	printf("Reseting 25 is done \r\n");

	for( j = 0; j < nodes_l26; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l25; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_23, out_Width_23, kernel_h_25, kernel_w_25, kernel_padding_25, kernel_stride_25, W_fxp_25, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_25 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_25 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l25; i++)
		{
//			if(in_block_w == 5 && in_block_h == 5)
//			{
//				in_arr_full++;
//				in_block_w = 0;
//				in_block_h = 0;
//			}
			for( p = 0; p < out_Width_23; p++)
			{
				for( q = 0; q < out_Hight_23; q++)
				{
					if((out_Width_23 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_23 * in_block_h);
						mm = p + (out_Width_23 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_23 - 1) && (q == out_Hight_23 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_23 - 1) && (q == out_Hight_23 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_23 * in_block_h);
						mm = p + (out_Width_23 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		if(out_block_w == 7 && out_block_h == 7)
		{
			out_arr_full++;
			out_block_w = 0;
			out_block_h = 0;
		}
		for( n = kernel_h_25 - 1; n < out_Hight_23 + kernel_stride_25; n = n + kernel_stride_25)
		{
			p = 0;
			for( m = kernel_w_25 - 1; m < out_Width_23 + kernel_stride_25; m = m + kernel_stride_25)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_25 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_25 * out_block_h);
					k = p + (out_Width_25 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_25 - 1) && (q == out_Hight_25 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_25 - 1) && (q == out_Hight_25 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_25 * out_block_h);
					k = p + (out_Width_25 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_25, out_Width_25, b_fxp_25, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;


	} //end nodes_l26 in R22-R25  (256, 13, 13)

	// Declaration
	static float W_flp_26 [nodes_l27][nodes_l26][kernel_h_26][kernel_w_26];
	static float b_flp_26 [nodes_l27];

	static int W_fxp_26 [nodes_l27][nodes_l26][kernel_h_26][kernel_w_26];
	static int b_fxp_26 [nodes_l27];

	max = -10000;

	// Initialization
	for (t1 = 0; t1 < nodes_l27; t1++)
	{
		for (t2 = 0; t2 < nodes_l26; t2++)
		{
			for (t3 = 0; t3 < kernel_h_26; t3++)
			{
				for (t4 = 0; t4 < kernel_w_26; t4++)
				{
					W_flp_26[t1][t2][t3][t4] = 0.03;
				}
			}
		}
	}
	for (t1 = 0; t1 < nodes_l27; t1++)
	{
		b_flp_26[t1] = 0.1;
	}
	printf("Initialization 26 is done \r\n");

	//converting to fixed-point
	FxP_Conversion_1D(nodes_l27, b_flp_26, b_fxp_26);
	FxP_Conversion_4D(nodes_l27, nodes_l26, kernel_h_26, kernel_w_26, W_flp_26, W_fxp_26);
	printf("Fixed_Point Conversion 26 is done \r\n");

	in_block_w = 0;
	in_block_h = 0;
	out_block_w = 0;
	out_block_h = 0;
	block_pool_w = 0;
	block_pool_h = 0;
	in_arr_full = 1;
	out_arr_full = 0;
	in_layer = 8;
	printf("Reseting 26 is done \r\n");

	for( j = 0; j < nodes_l27; j++)
	{
		// Write W into the right Block
		for( i = 0; i < nodes_l26; i++)
		{
			WRITE_Matrix_Weight(j, i, out_Hight_25, out_Width_25, kernel_h_26, kernel_w_26, kernel_padding_26, kernel_stride_26, W_fxp_26, 2);
		}

		// Do Convolution
		if(j == 0)
		{
			for( i = 0; i < in_layer; i++)
			{
				execute(2, 3, 17 + i, 2);														//R3 = R1 * R2
				mult_cnt++;
				execute(7, 4, 3, 0);																	//Sh_S
				ShS_cnt++;
				execute(0, 5, 4, 3);
				add_cnt++;
				for( q = 0; q < kernel_h_26 - 2; q++)
				{
					execute(7, 6, 5, 0);																//Sh_S
					ShS_cnt++;
					execute(0, 5, 6, 3);																//Add
					add_cnt++;
				}
				execute(5, 6, 5, 0);																	//Sh_E
				ShR_cnt++;
				execute(0, 7, 6, 5);
				add_cnt++;
				for( q = 0; q < kernel_w_26 - 2; q++)
				{
					execute(5, 8, 7, 0);																//Sh_E
					ShR_cnt++;
					execute(0, 7, 8, 5);																//Add
					add_cnt++;
				}
				execute(0, 17 + i, 7, 0);
				add_cnt++;
			}
		}

		// Move to the top left and add each time
		in_block_w = 0;
		in_block_h = 0;
		for( i = 0; i < nodes_l25 + nodes_l26; i++)
		{
			if(in_block_w == 7 && in_block_h == 7)
			{
				in_arr_full++;
				in_block_w = 0;
				in_block_h = 0;
			}
			for( p = 0; p < out_Width_25; p++)
			{
				for( q = 0; q < out_Hight_25; q++)
				{
					if((out_Width_25 * in_block_w) < block_dim * 4)
					{
						nn = q + (out_Hight_25 * in_block_h);
						mm = p + (out_Width_25 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
						if((p == out_Width_25 - 1) && (q == out_Hight_25 - 1))
						{
							in_block_w++;
						}
					}
					else
					{
						if((p == out_Width_25 - 1) && (q == out_Hight_25 - 1))
						{
							in_block_h++;
							in_block_w = 0;
						}

						nn = q + (out_Hight_25 * in_block_h);
						mm = p + (out_Width_25 * in_block_w);

						int tmp = READ_REG(nn/4, mm/4, ((nn*4+mm)%4+nn*4)%16, 17 + in_arr_full);
						WRITE_REG(q/4, p/4, ((q*4+p)%4+q*4)%16, 14, tmp);
					}
				}
			}
			execute(0, 17 + in_arr_full, 14, 17 + in_arr_full);
			add_cnt++;
		}

		// Select final result and rewrite to the right output block position
		k = 0;
		l = 0;
		p = 0;
		q = 0;
		for( n = kernel_h_26 - 1; n < out_Hight_25 + kernel_stride_26; n = n + kernel_stride_26)
		{
			p = 0;
			for( m = kernel_w_26 - 1; m < out_Width_25 + kernel_stride_26; m = m + kernel_stride_26)
			{
				unsigned int tmp_out = READ_REG(n/4, m/4, ((n*4+m)%4+n*4)%16, 17 + in_arr_full);
				if((out_Width_26 * out_block_w) < block_dim * 4)
				{
					l = q + (out_Hight_26 * out_block_h);
					k = p + (out_Width_26 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);

					if((p == out_Width_26 - 1) && (q == out_Hight_26 - 1))
					{
						out_block_w++;
					}
				}
				else
				{
					if((p == out_Width_26 - 1) && (q == out_Hight_26 - 1))
					{
						out_block_h++;
						out_block_w = 0;
					}

					l = q + (out_Hight_26 * out_block_h);
					k = p + (out_Width_26 * out_block_w);

					WRITE_REG(l/4, k/4, ((l*4+k)%4+l*4)%16, 17 + out_arr_full, tmp_out);
				}
				p++;
			}
			q++;
		}


		// Add bias
		WRITE_Matrix_Bias(j, out_Hight_26, out_Width_26, b_fxp_26, 11); 										//b in R11
		execute(0, 17 + out_arr_full, 11, 17 + out_arr_full);
		add_cnt++;
	} //end nodes_l27 in R17 (10, 13, 13)

	printf("Number of ShR: %d \r\n", ShR_cnt);
	printf("Number of ShL: %d \r\n", ShL_cnt);
	printf("Number of ShN: %d \r\n", ShN_cnt);
	printf("Number of ShS: %d \r\n", ShS_cnt);
	printf("Number of add: %d \r\n", add_cnt);
	printf("Number of mult: %d \r\n", mult_cnt);
	printf("Number of write_stall: %d \r\n", write_stall_cnt);
	printf("Number of read_stall: %d \r\n", read_stall_cnt);

	return 0;

}
