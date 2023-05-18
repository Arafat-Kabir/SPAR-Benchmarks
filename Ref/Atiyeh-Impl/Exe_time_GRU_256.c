/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
//#include "platform.h"
//#include "printf.h"
#include <stdlib.h>
#include <stdint.h>
//#include "ARPMB.h"
//#include "xparameters.h"
//#include "microblaze_sleep.h"
//#include "xtmrctr.h"
#include <inttypes.h>
//#include "xbasic_types.h"
#include "math.h"
//#include "xil_io.h"

#define block_dim 25
#define divide_size_W 39
#define divide_size_U 64
#define m1 256							//size of hidden layers
#define p 39							//size of inputs
//#define q 39							//size of outputs

#define MAX_LEN 32
#define FIXED_POINT_FRACTIONAL_BITS 16

int ShL_cnt = 0;
int ShR_cnt = 0;
int ShN_cnt = 0;
int ShS_cnt = 0;
int mult_cnt = 0;
int add_cnt = 0;
int write_stall_cnt = 0;

//Function prototypes
int execute(int opcode, int rd, int rs1, int rs2);
//int readRAM(int address);
int getbit(int n, int k);
void getTranspose(int* ram, int* reg);
void printArray(int* arr, int size);
void getData(int* arr, int address);
unsigned char Test(int64_t* rd_reg, int64_t* rs1_reg, int64_t* rs2_reg, char operation);
void printRegFile();
void WRITE(int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB);
int READ(int BRAM_i, int BRAM_j, int ADDRA, int ADDRB);
int printReg(int BRAM_i, int BRAM_j, int PE, int reg);
int getTrans(int* ram);
void printRegFile(int BRAM_i, int BRAM_j, int number_of_regs);
void WRITE_REG(int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data);
void FxP_Conversion(int row, int col, void *in_temp, void *out_temp );

//Fixed-point Format: 11.5 (16-bit)
typedef int32_t fixed_point_t;

//Converts double to 11.5 format
fixed_point_t float_to_fixed(double input);

int LEN = MAX_LEN << 24;

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
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			printf("%08x\t", arr[i][j]);
		}
		printf("\n\r");
	}
}

inline fixed_point_t float_to_fixed(double input)
{
    return (fixed_point_t)(input * (1 << FIXED_POINT_FRACTIONAL_BITS));
}

void FxP_Conversion(int row, int col, void *in_temp, void *out_temp ){

	float (*in)[col] = (float (*)[col]) in_temp;
	int (*out)[col] = (int (*)[col]) out_temp;

	int s, t;
	for(s=0; s<row; s++)
	{
		for(t=0; t<col; t++)
		{
			out[s][t] = float_to_fixed(in[s][t]);
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
	for(i = 0; i < matrixA_col-2; i++)
	{
		execute(5,23,22,0);					//ShiftEast
		ShR_cnt++;
		execute(0,22,23,21);				//Add
		add_cnt++;
	}
	//shift till edge
	for(i = 0; i < (block_dimension*4) - matrixA_col; i++)
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
	for(i=0; i<log_colA; i++){
		execute(5,22,21,0);						//ShiftEast
		col_cnt++;
		ShR_cnt++;
		for(j=0; j<pow((double)2,(double)i)-1; j++){
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
		for(i = col_cnt; i < matrixA_col; i++)
		{
			execute(5,22,21,0);					//ShiftEast
			ShR_cnt++;
			execute(0,21,23,22);				//Add
			add_cnt++;
		}
	}

	//shift till edge
	for(i = 0; i < (block_dimension*4) - matrixA_col; i++)
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

void WRITE_Matrix_Large(int row, int col, int block_row, int block_col, int row_blk_size, int col_blk_size, int W[][col], int reg, int copy, int block_dimension){


	int s, t, i;
	if (copy == 1)		//write a 2D array, starts from the left-most PEs
	{
		for(s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for(t = col_blk_size * block_col; t < ((block_col + 1) * col_blk_size); t++)
			{
				WRITE_REG((s - (row_blk_size * block_row))/4, (t - (col_blk_size * block_col))/4, (((s - (row_blk_size * block_row))*4 + (t - (col_blk_size * block_col))) % 4 + (s - (row_blk_size * block_row)) * 4)%16, reg, W[s][t]);
				write_stall_cnt++;
			}
		}
	}
	else if(copy == 0)	//write a 1D array into last column PEs
	{
		for(s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for(t = col_blk_size * block_col; t < col; t++)
			{
				WRITE_REG((s - (row_blk_size * block_row))/4, block_dimension-1, ((s - (row_blk_size * block_row))*4+3)%16, reg, W[s][t]);
				write_stall_cnt++;
			}
		}
	}
	else if(copy == -1)	//write a 1D array into first column PEs
	{
		for(s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for(t = col_blk_size * block_col; t < col; t++)
			{
				WRITE_REG(0, (s - (row_blk_size * block_row))/4, (s - (row_blk_size * block_row))%4, reg, W[s][t]);
				write_stall_cnt++;
			}
		}
	}
	else				//write a 1D array into first row, copies to the below PEs too
	{
		for(s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for(t = col_blk_size * block_col; t < col; t++)
			{
				WRITE_REG((s - (row_blk_size * block_row))/4, block_dimension-1, ((s - (row_blk_size * block_row))*4+3)%16, reg, W[s][t]);
				write_stall_cnt++;
			}
		}
		ColumnToRow(reg, reg, 0);

		int instruction = (7<<26) + (reg<<21) + (reg << 16) + (0 << 11);
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
		for(i = 0; i < copy-1; i++)
		{
//			ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//			ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		}

	}

}

void GATE_Large(int rowX, int colX, int rowW, int colW, int rowU, int colU, int rowH, int colH, int rowb, int colb,
		  		  int X[][colX], int W[][colW], int U[][colU], int H[][colH], int b[][colb],
		  		  int block_dimension, int div_size_W, int div_size_U,
				  int res_Reg_0, int res_Reg_1, int res_Reg_2, int res_Reg_3, int function, int layer){

	if(layer == 0)
	{
		WRITE_Matrix_Large(rowX, 1, 0, 0, div_size_W, div_size_W, X, 1, rowW, block_dimension);			//X in R1

		WRITE_Matrix_Large(rowW, colW, 0, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(1, 2, 3, div_size_W, block_dimension);								//WX in R3
		printf("WX_0 is done \n\r");

		WRITE_Matrix_Large(rowW, colW, 1, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(1, 2, 4, div_size_W, block_dimension);								//WX in R4
		printf("WX_1 is done \n\r");

		WRITE_Matrix_Large(rowW, colW, 2, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(1, 2, 5, div_size_W, block_dimension);								//WX in R5
		printf("WX_2 is done \n\r");

		WRITE_Matrix_Large(rowW, colW, 3, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(1, 2, 6, div_size_W, block_dimension);								//WX in R6
		printf("WX_3 is done \n\r");

	}
	else
	{
		WRITE_Matrix_Large(rowW, colW, 0, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(27, 2, 3, div_size_W, block_dimension);								//WX in R3

		WRITE_Matrix_Large(rowW, colW, 1, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(28, 2, 4, div_size_W, block_dimension);								//WX in R4

		WRITE_Matrix_Large(rowW, colW, 2, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(29, 2, 5, div_size_W, block_dimension);								//WX in R5

		WRITE_Matrix_Large(rowW, colW, 3, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(30, 2, 6, div_size_W, block_dimension);								//WX in R6

	}

	WRITE_Matrix_Large(rowH, colH, 0, 0, div_size_U, div_size_U, H, 10, div_size_U, block_dimension);
	printf("H_0 is done \n\r");

	WRITE_Matrix_Large(rowH, colH, 1, 0, div_size_U, div_size_U, H, 11, div_size_U, block_dimension);
	printf("H_1 is done \n\r");

	WRITE_Matrix_Large(rowH, colH, 2, 0, div_size_U, div_size_U, H, 12, div_size_U, block_dimension);
	printf("H_2 is done \n\r");

	WRITE_Matrix_Large(rowH, colH, 3, 0, div_size_U, div_size_U, H, 13, div_size_U, block_dimension);
	printf("H_3 is done \n\r");


	WRITE_Matrix_Large(rowU, colU, 0, 0, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(10, 8, 9, div_size_U, block_dimension);		//UH1 in R9

	WRITE_Matrix_Large(rowU, colU, 0, 1, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(11, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(9, 2, 9);

	WRITE_Matrix_Large(rowU, colU, 0, 2, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(12, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(9, 2, 9);

	WRITE_Matrix_Large(rowU, colU, 0, 3, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(13, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(9, 2, 9);

	printf("UH_0 is done \n\r");

	WRITE_Matrix_Large(rowU, colU, 1, 0, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(10, 8, 15, div_size_U, block_dimension);		//UH1 in R9

	WRITE_Matrix_Large(rowU, colU, 1, 1, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(11, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(15, 2, 15);

	WRITE_Matrix_Large(rowU, colU, 1, 2, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(12, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(15, 2, 15);

	WRITE_Matrix_Large(rowU, colU, 1, 3, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(13, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(15, 2, 15);

	printf("UH_1 is done \n\r");

	WRITE_Matrix_Large(rowU, colU, 2, 0, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(10, 8, 16, div_size_U, block_dimension);		//UH1 in R9

	WRITE_Matrix_Large(rowU, colU, 2, 1, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(11, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(16, 2, 16);

	WRITE_Matrix_Large(rowU, colU, 2, 2, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(12, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(16, 2, 16);

	WRITE_Matrix_Large(rowU, colU, 2, 3, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(13, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(16, 2, 16);

	printf("UH_2 is done \n\r");

	WRITE_Matrix_Large(rowU, colU, 3, 0, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(10, 8, 17, div_size_U, block_dimension);		//UH1 in R9

	WRITE_Matrix_Large(rowU, colU, 3, 1, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(11, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(17, 2, 17);

	WRITE_Matrix_Large(rowU, colU, 3, 2, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(12, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(17, 2, 17);

	WRITE_Matrix_Large(rowU, colU, 3, 3, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(13, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(17, 2, 17);

	printf("UH_3 is done \n\r");


	MARTIX_ADDITION(3, 9, 3);										//WX + UH (1) in R3
	MARTIX_ADDITION(4, 15, 4);										//WX + UH (2) in R4
	MARTIX_ADDITION(5, 16, 5);										//WX + UH (1) in R5
	MARTIX_ADDITION(6, 17, 6);										//WX + UH (2) in R6

	WRITE_Matrix_Large(rowb, colb, 0, 0, div_size_U, div_size_U, b, 9, 0, block_dimension);
	WRITE_Matrix_Large(rowb, colb, 1, 0, div_size_U, div_size_U, b, 15, 0, block_dimension);
	WRITE_Matrix_Large(rowb, colb, 2, 0, div_size_U, div_size_U, b, 16, 0, block_dimension);
	WRITE_Matrix_Large(rowb, colb, 3, 0, div_size_U, div_size_U, b, 17, 0, block_dimension);
	printf("b is done \n\r");

	MARTIX_ADDITION(3, 9, 3);										//WX + UH + b in R3
	MARTIX_ADDITION(4, 15, 4);										//WX + UH + b in R4
	MARTIX_ADDITION(5, 16, 5);										//WX + UH + b in R5
	MARTIX_ADDITION(6, 17, 6);										//WX + UH + b in R6
	printf("WX + UH + b is done \n\r");

	ColumnToRow(res_Reg_0, 3, function);
	ColumnToRow(res_Reg_1, 4, function);
	ColumnToRow(res_Reg_2, 5, function);
	ColumnToRow(res_Reg_3, 6, function);
}

void WRITE_Matrix(int row, int col, int W[][col], int reg, int copy, int block_dimension){

	int s, t, i;
	if (copy == 1)		//write a 2D array, starts from the left-most PEs
	{
		for(s=0; s<row; s++)
		{
			for(t=0; t<col; t++)
			{
				WRITE_REG(s/4, t/4, ((s*4+t)%4+s*4)%16, reg, W[s][t]);
			}
		}
	}
	else if(copy == 0)	//write a 1D array into last column PEs
	{
		for(s=0; s<row; s++)
		{
			for(t=0; t<col; t++)
			{
				WRITE_REG(s/4, block_dimension-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
	}
	else if(copy == -1)	//write a 1D array into first column PEs
	{
		for(s=0; s<row; s++)
		{
			for(t=0; t<col; t++)
			{
				WRITE_REG(0, s/4, s%4, reg, W[s][t]);
			}
		}
	}
	else				//write a 1D array into first row, copies to the below PEs too
	{
		for(s=0; s<row; s++)
		{
			for(t=0; t<col; t++)
			{
				WRITE_REG(s/4, block_dimension-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
		ColumnToRow(reg, reg, 0);

		int instruction = (7<<26) + (reg<<21) + (reg << 16) + (0 << 11);
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
		for(i = 0; i < copy-1; i++)
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
	for(i=base-2; i>=base-33; i-=2){

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
	for(i=0; i<16; i++){
		DO = READ (BRAM_i, BRAM_j, reg+i, reg+i+16);
		REG[i+16] = getbit(DO, PE);
		REG[i] = getbit(DO, PE+16);
	}

	reg_out = getTrans(REG);

	return reg_out;
}

void printRegFile(int BRAM_i, int BRAM_j, int number_of_regs){

	int PE, i;
	for(PE=0; PE<16; PE++){

		for(i=number_of_regs-1; i>=0; i--)
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
		//init_platform();
//		arr[i] = readRAM(address+i);
		//cleanup_platform();
	}
}


int main()
{
	//init_platform();
	//cleanup_platform();

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
	for(i = 0; i < block_dim; i++){
		for(j = 0; j < block_dim; j++){
			for(addra = 0; addra < 32*32; addra++){
				WRITE(i, j, addra, 0, 0, 0);
			}
		}
	}


	static float f_Wi_1[m1][p] =
	{

	};
	static float f_Wf_1[m1][p];
	static float f_Wg_1[m1][p];
	static float f_Wo_1[m1][p];
	static float f_Ui_1[m1][m1] =
	{
	};
	static float f_Uf_1[m1][m1];
	static float f_Ug_1[m1][m1];
	static float f_Uo_1[m1][m1];
	static float f_bi_1[m1][1] =
	{
	};
	static float f_bf_1[m1][1];
	static float f_bg_1[m1][1];
	static float f_bo_1[m1][1];
//	static float f_W_FC[q][m3];
//	static float f_b_FC[q][1];

	static float f_Hc_1[m1][1] = {0};
	static float f_H_t_1_1[m1][1] = {0};

	static int X[p][1] = {{0x00010000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},
								{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},
								{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},
								{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000},{0x00000000}
								};


	static int Wi_1[m1][p];
	static int Wf_1[m1][p];
	static int Wg_1[m1][p];
	static int Wo_1[m1][p];

	static int Ui_1[m1][m1];
	static int Uf_1[m1][m1];
	static int Ug_1[m1][m1];
	static int Uo_1[m1][m1];

	static int bi_1[m1][1];
	static int bf_1[m1][1];
	static int bg_1[m1][1];
	static int bo_1[m1][1];

	static int Hc_1[m1][1];
	static int H_t_1_1[m1][1];
//	static int W_FC[q][m3];
//	static int b_FC[q][1];
	printf("MATRIX DEFINITION IS DONE! \n");

	// FxP_Conversion(p, 1, f_X, X);
	FxP_Conversion(m1, 1, f_bi_1, bi_1);
	FxP_Conversion(m1, 1, f_bf_1, bf_1);
	FxP_Conversion(m1, 1, f_bg_1, bg_1);
	FxP_Conversion(m1, 1, f_bo_1, bo_1);

	FxP_Conversion(m1, p, f_Wi_1, Wi_1);
	FxP_Conversion(m1, p, f_Wf_1, Wf_1);
	FxP_Conversion(m1, p, f_Wg_1, Wg_1);
	FxP_Conversion(m1, p, f_Wo_1, Wo_1);

	FxP_Conversion(m1, m1, f_Ui_1, Ui_1);
	FxP_Conversion(m1, m1, f_Uf_1, Uf_1);
	FxP_Conversion(m1, m1, f_Ug_1, Ug_1);
	FxP_Conversion(m1, m1, f_Uo_1, Uo_1);

	FxP_Conversion(m1, 1, f_Hc_1, Hc_1);
	FxP_Conversion(m1, 1, f_H_t_1_1, H_t_1_1);
//	FxP_Conversion(q, m3, f_W_FC, W_FC);
//	FxP_Conversion(q, 1, f_b_FC, b_FC);
	printf("FIXED POINT CONVERSION IS DONE! \n");

	// First LSTM Layer
	GATE_Large(p, 1, m1, p, m1, m1, m1, 1, m1, 1, X, Wi_1, Ui_1, H_t_1_1, bi_1, block_dim, divide_size_W, divide_size_U, 8, 9, 10, 11, 1, 0);
	GATE_Large(p, 1, m1, p, m1, m1, m1, 1, m1, 1, X, Wg_1, Ug_1, H_t_1_1, bg_1, block_dim, divide_size_W, divide_size_U, 18, 19, 20, 24, 2, 0);

	ELEMENTWISE_MULTIPLICATION(8, 18, 8);								//HiHg in R8
	ELEMENTWISE_MULTIPLICATION(9, 19, 9);								//HiHg in R9
	ELEMENTWISE_MULTIPLICATION(10, 20, 10);								//HiHg in R10
	ELEMENTWISE_MULTIPLICATION(11, 24, 11);								//HiHg in R11

	GATE_Large(p, 1, m1, p, m1, m1, m1, 1, m1, 1, X, Wf_1, Uf_1, H_t_1_1, bf_1, block_dim, divide_size_W, divide_size_U, 13, 14, 15, 16, 1, 0);
	WRITE_Matrix_Large(m1, 1, 0, 0, divide_size_U, divide_size_U, Hc_1, 18, -1, block_dim);
	WRITE_Matrix_Large(m1, 1, 1, 0, divide_size_U, divide_size_U, Hc_1, 19, -1, block_dim);
	WRITE_Matrix_Large(m1, 1, 2, 0, divide_size_U, divide_size_U, Hc_1, 20, -1, block_dim);
	WRITE_Matrix_Large(m1, 1, 3, 0, divide_size_U, divide_size_U, Hc_1, 24, -1, block_dim);

	ELEMENTWISE_MULTIPLICATION(13, 18, 13);								//HfHc in R13
	ELEMENTWISE_MULTIPLICATION(14, 19, 14);								//HfHc in R14
	ELEMENTWISE_MULTIPLICATION(15, 20, 15);								//HfHc in R15
	ELEMENTWISE_MULTIPLICATION(16, 24, 16);								//HfHc in R16

	MARTIX_ADDITION(8, 13, 8);											//Hc = HiHg + HfHc in R8
	MARTIX_ADDITION(9, 14, 9);											//Hc = HiHg + HfHc in R9
	MARTIX_ADDITION(10, 15, 10);										//Hc = HiHg + HfHc in R10
	MARTIX_ADDITION(11, 16, 11);										//Hc = HiHg + HfHc in R11

	RowToRow(8, 8);
	int instruction = (7 << 26) + (8 << 21) + (8 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}
	RowToRow(9, 9);
	instruction = (7 << 26) + (9 << 21) + (9 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}
	RowToRow(10, 10);
	instruction = (7 << 26) + (10 << 21) + (10 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}
	RowToRow(11, 11);
	instruction = (7 << 26) + (11 << 21) + (11 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}

	GATE_Large(p, 1, m1, p, m1, m1, m1, 1, m1, 1, X, Wo_1, Uo_1, H_t_1_1, bo_1, block_dim, divide_size_W, divide_size_U, 18, 19, 20, 24, 1, 0);
	RowToRow(18, 18);
	instruction = (7 << 26) + (18 << 21) + (18 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}
	RowToRow(19, 19);
	instruction = (7 << 26) + (19 << 21) + (19 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}
	RowToRow(20, 20);
	instruction = (7 << 26) + (20 << 21) + (20 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}
	RowToRow(24, 24);
	instruction = (7 << 26) + (24 << 21) + (24 << 16) + (0 << 11);
	for( i = 0; i < divide_size_U; i++){
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//		ARPMB_mWriteReg(XPAR_ARPMB_0_S00_AXI_BASEADDR, ARPMB_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		ShS_cnt++;
	}

	ELEMENTWISE_MULTIPLICATION(27, 8, 18);									//H = HcHo in R27
	ELEMENTWISE_MULTIPLICATION(28, 9, 19);									//H = HcHo in R28
	ELEMENTWISE_MULTIPLICATION(29, 10, 20);									//H = HcHo in R29
	ELEMENTWISE_MULTIPLICATION(30, 11, 24);									//H = HcHo in R30

	printf("Number of ShR: %d \r\n", ShR_cnt);
	printf("Number of ShL: %d \r\n", ShL_cnt);
	printf("Number of ShN: %d \r\n", ShN_cnt);
	printf("Number of ShS: %d \r\n", ShS_cnt);
	printf("Number of add: %d \r\n", add_cnt);
	printf("Number of mult: %d \r\n", mult_cnt);
	printf("Number of write_stall: %d \r\n", write_stall_cnt);

	return 0;
}

