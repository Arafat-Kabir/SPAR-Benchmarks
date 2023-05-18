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
//#include "xil_printf.h"
#include <stdlib.h>
#include <stdint.h>
//#include "platform.h"
//#include "ARP_WO_DFF.h"
//#include "xparameters.h"
//#include "microblaze_sleep.h"
//#include "xtmrctr.h"
#include <inttypes.h>
//#include "xbasic_types.h"
#include "math.h"

#define MAX_LEN 32
#define FIXED_POINT_FRACTIONAL_BITS 16

#define Array_dim  1
#define Tile_dim  28
#define divide_size_W 112
#define divide_size_U 100
#define m1 100						//size of hidden layers
#define p 784							//size of inputs
#define q 100							//size of outputs

//#define m1 9						//size of hidden layers
//#define m2 16						//size of hidden layers
//#define m3 48						//size of hidden layers
//#define m4 40						//size of hidden layers
//#define m5 16						//size of hidden layers
//#define p 100							//size of inputs
//#define q 2							//size of outputs

int ShL_cnt = 0;
int ShR_cnt = 0;
int ShN_cnt = 0;
int ShS_cnt = 0;
int mult_cnt = 0;
int add_cnt = 0;

//Function prototypes
int execute(int opcode, int rd, int rs1, int rs2);
int readRAM(int address);
int getbit(int n, int k);
void getTranspose(int* ram, int* reg);
void printArray(int* arr, int size);
void getData(int* arr, int address);
unsigned char Test(int64_t* rd_reg, int64_t* rs1_reg, int64_t* rs2_reg, char operation);
void WRITE(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB);
int READ(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB);
int printReg(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg);
int getTrans(int* ram);
void printRegFile(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int number_of_regs);
void WRITE_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data);

//Fixed-point Format: 11.5 (16-bit)
typedef int32_t fixed_point_t;

//Converts double to 11.5 format
fixed_point_t float_to_fixed(double input);

int LEN = MAX_LEN << 24;

int main()
{
	//init_platform();
	//cleanup_platform();

	printf("Program Started...\n\n\r");

	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG0_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);

	//reset = 1
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);

	//reset all the registers in the array to zero
	int i, j, k, l, addra;
	for( i = 0; i < Array_dim; i++){
		for( j = 0; j < Array_dim; j++){
			for( k = 0; k < Tile_dim; k++){
				for( l = 0; l < Tile_dim; l++){
					for( addra = 0; addra < 32 * 32; addra++){
						WRITE(i, j, k, l, addra, 0, 0, 0);
					}
				}
			}
		}
	}
	printf("RESET REGISTERS IS DONE! \n");

	static float X_f[p][1];
	static float W1_f[m1][p];
	static float W2_f[q][m1];
	static float b1_f[m1][1];
	static float b2_f[q][1];

	static int X[p][1];
	static int W1[m1][p];
	static int W2[q][m1];
	static int b1[m1][1];
	static int b2[q][1];

	// MLP
	MARTIX_MULTIPLICATION(15, 19, 16, divide_size_W, Array_dim * Tile_dim);				//WX in R8
	MARTIX_MULTIPLICATION(15, 19, 16, divide_size_W, Array_dim * Tile_dim);				//WX in R9
	MARTIX_MULTIPLICATION(15, 19, 16, divide_size_W, Array_dim * Tile_dim);				//WX in R10
	MARTIX_MULTIPLICATION(15, 19, 16, divide_size_W, Array_dim * Tile_dim);				//WX in R8
	MARTIX_MULTIPLICATION(15, 19, 16, divide_size_W, Array_dim * Tile_dim);				//WX in R9
	MARTIX_MULTIPLICATION(15, 19, 16, divide_size_W, Array_dim * Tile_dim);				//WX in R8
	MARTIX_MULTIPLICATION(15, 19, 16, divide_size_W, Array_dim * Tile_dim);				//WX in R9

	MARTIX_ADDITION(16, 20, 17);															//WX + b in R13
	MARTIX_ADDITION(16, 20, 17);															//WX + b in R14
	MARTIX_ADDITION(16, 20, 17);															//WX + b in R15
	MARTIX_ADDITION(16, 20, 17);															//WX + b in R13
	MARTIX_ADDITION(16, 20, 17);															//WX + b in R14
	MARTIX_ADDITION(16, 20, 17);															//WX + b in R13
	MARTIX_ADDITION(16, 20, 17);															//WX + b in R14
	
	ColumnToRow(17, 17, 1);
	int instruction = (7<<26) + (17<<21) + (17<< 16) + (0 << 11);
	for( i=1; i<q; i++){
		ShS_cnt++;
	}
	
	MARTIX_MULTIPLICATION(17, 21, 22, m1, Array_dim * Tile_dim);	//W2H in R22
	MARTIX_ADDITION(22, 23, 24);											//H = W1X+b in R24										
	
	printf("Number of ShR: %d \r\n", ShR_cnt);
	printf("Number of ShL: %d \r\n", ShL_cnt);
	printf("Number of ShN: %d \r\n", ShN_cnt);
	printf("Number of ShS: %d \r\n", ShS_cnt);
	printf("Number of add: %d \r\n", add_cnt);
	printf("Number of mult: %d \r\n", mult_cnt);

    return 0;
}

void ColumnToRow(int rd, int rs, int function){

	//function: 0 == noOp, 1 == sigmoid, 2 == tanh
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG6_OFFSET, function);
	execute(5, rs, rs, 0);
	ShR_cnt++;
	int instruction = (7<<26) + (rd<<21) + (rd << 16) + (0 << 11);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	ShS_cnt++;
}

void RowToRow(int rd, int rs){

	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG6_OFFSET, 0x6);
	execute(8, rs, rs, 0);
	ShN_cnt++;
	int instruction = (7<<26) + (rd<<21) + (rd << 16) + (0 << 11);

	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
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

void FxP_Conversion(int row, int col, float in[][col], int out[][col] ){

	int s, t;
	for( s=0; s<row; s++)
	{
		for( t=0; t<col; t++)
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

	int i;
	execute(2,21,matrixB_reg,matrixA_reg);	//Mult
	mult_cnt++;
	execute(5,27,21,0);						//ShiftEast
	ShR_cnt++;
	execute(0,22,27,21);					//Add
	add_cnt++;
	//shift and add
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
		execute(0,21,22,21);					//Add
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

void MARTIX_MULTIPLICATION_Optimized_Shift(int matrixA_reg, int matrixB_reg, int Result_Reg, int matrixA_col, int block_dimension){

	unsigned int log_colA = Log2n(matrixA_col);
	int col_cnt = 0;
	execute(2,21,matrixB_reg,matrixA_reg);	//Mult
	mult_cnt++;
	execute(0,23,21,0);						//Add
	add_cnt++;

	int i, j;
	for(i=0; i<log_colA; i++){
		execute(5,22,21,0);						//ShiftEast
		col_cnt = col_cnt + (pow((double)2,(double)i));
		ShR_cnt++;
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
	unsigned int log_diff_colA = Log2n((block_dimension*4) - matrixA_col);
	execute(5,21,21,0);					//ShiftEast
	ShR_cnt++;
	for(i = 0; i < ((block_dimension*4) - matrixA_col - (pow((double)2,(double)log_diff_colA))); i++)
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

	//MARTIX_MULTIPLICATION(X_reg, W_reg, 25, colW, block_dimension);		//WX in R25
	//MARTIX_MULTIPLICATION(H_reg, U_reg, 26, colU, block_dimension);		//UH in R26

	MARTIX_MULTIPLICATION_Optimized(X_reg, W_reg, 25, colW, block_dimension);		//WX in R25
	MARTIX_MULTIPLICATION_Optimized(H_reg, U_reg, 26, colU, block_dimension);		//UH in R26
	MARTIX_ADDITION(25, 26, 25);										//WX + UH in R25
	MARTIX_ADDITION(25, b_reg, 25);										//WX + UH + b in R25
	ColumnToRow(Resultreg, 25, function);
}

void GATE_Large(int rowX, int colX, int rowW, int colW, int rowU, int colU, int rowH, int colH, int rowb, int colb,
		  		  int X[][colX], int W[][colW], int U[][colU], int H[][colH], int b[][colb],
		  		  int block_dimension, int div_size_W, int div_size_U,
				  int res_Reg_0, int res_Reg_1, int res_Reg_2, int res_Reg_3, int res_Reg_4, int function, int layer){

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

		WRITE_Matrix_Large(rowW, colW, 4, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(1, 2, 7, div_size_W, block_dimension);								//WX in R7
		printf("WX_4 is done \n\r");

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

		WRITE_Matrix_Large(rowW, colW, 4, 0, div_size_U, div_size_W, W, 2, 1, block_dimension);			//W in R2
		MARTIX_MULTIPLICATION_Optimized(31, 2, 7, div_size_W, block_dimension);
	}

	WRITE_Matrix_Large(rowH, colH, 0, 0, div_size_U, div_size_U, H, 10, div_size_U, block_dimension);
	printf("H_0 is done \n\r");

	WRITE_Matrix_Large(rowH, colH, 1, 0, div_size_U, div_size_U, H, 11, div_size_U, block_dimension);
	printf("H_1 is done \n\r");

	WRITE_Matrix_Large(rowH, colH, 2, 0, div_size_U, div_size_U, H, 12, div_size_U, block_dimension);
	printf("H_2 is done \n\r");

	WRITE_Matrix_Large(rowH, colH, 3, 0, div_size_U, div_size_U, H, 13, div_size_U, block_dimension);
	printf("H_3 is done \n\r");

	WRITE_Matrix_Large(rowH, colH, 4, 0, div_size_U, div_size_U, H, 14, div_size_U, block_dimension);
	printf("H_4 is done \n\r");


	WRITE_Matrix_Large(rowU, colU, 0, 0, div_size_U, div_size_U, U, 8, 1, block_dimension); //was 8
	MARTIX_MULTIPLICATION_Optimized(10, 8, 9, div_size_U, block_dimension);		//UH1 in R9		//was 8

	WRITE_Matrix_Large(rowU, colU, 0, 1, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(11, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(9, 2, 9);

	WRITE_Matrix_Large(rowU, colU, 0, 2, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(12, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(9, 2, 9);

	WRITE_Matrix_Large(rowU, colU, 0, 3, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(13, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(9, 2, 9);

	WRITE_Matrix_Large(rowU, colU, 0, 4, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(14, 8, 2, div_size_U, block_dimension);		//UH1 in R10
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

	WRITE_Matrix_Large(rowU, colU, 1, 4, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(14, 8, 2, div_size_U, block_dimension);		//UH1 in R10
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

	WRITE_Matrix_Large(rowU, colU, 2, 4, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(14, 8, 2, div_size_U, block_dimension);		//UH1 in R10
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

	WRITE_Matrix_Large(rowU, colU, 3, 4, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(14, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(17, 2, 17);
	printf("UH_3 is done \n\r");

	WRITE_Matrix_Large(rowU, colU, 4, 0, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(10, 8, 18, div_size_U, block_dimension);		//UH1 in R9

	WRITE_Matrix_Large(rowU, colU, 4, 1, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(11, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(18, 2, 18);

	WRITE_Matrix_Large(rowU, colU, 4, 2, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(12, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(18, 2, 18);

	WRITE_Matrix_Large(rowU, colU, 4, 3, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(13, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(18, 2, 18);

	WRITE_Matrix_Large(rowU, colU, 4, 4, div_size_U, div_size_U, U, 8, 1, block_dimension);
	MARTIX_MULTIPLICATION_Optimized(14, 8, 2, div_size_U, block_dimension);		//UH1 in R10
	MARTIX_ADDITION(18, 2, 18);
	printf("UH_4 is done \n\r");

	MARTIX_ADDITION(3, 9, 3);										//WX + UH (1) in R3
	MARTIX_ADDITION(4, 15, 4);										//WX + UH (2) in R4
	MARTIX_ADDITION(5, 16, 5);										//WX + UH (1) in R5
	MARTIX_ADDITION(6, 17, 6);										//WX + UH (2) in R6
	MARTIX_ADDITION(7, 18, 7);										//WX + UH (1) in R7

	WRITE_Matrix_Large(rowb, colb, 0, 0, div_size_U, div_size_U, b, 9, 0, block_dimension);
	WRITE_Matrix_Large(rowb, colb, 1, 0, div_size_U, div_size_U, b, 15, 0, block_dimension);
	WRITE_Matrix_Large(rowb, colb, 2, 0, div_size_U, div_size_U, b, 16, 0, block_dimension);
	WRITE_Matrix_Large(rowb, colb, 3, 0, div_size_U, div_size_U, b, 17, 0, block_dimension);
	WRITE_Matrix_Large(rowb, colb, 4, 0, div_size_U, div_size_U, b, 18, 0, block_dimension);
	printf("b is done \n\r");

	MARTIX_ADDITION(3, 9, 3);										//WX + UH + b in R3
	MARTIX_ADDITION(4, 15, 4);										//WX + UH + b in R4
	MARTIX_ADDITION(5, 16, 5);										//WX + UH + b in R5
	MARTIX_ADDITION(6, 17, 6);										//WX + UH + b in R6
	MARTIX_ADDITION(7, 18, 7);										//WX + UH + b in R7
	printf("WX + UH + b is done \n\r");

	ColumnToRow(res_Reg_0, 3, function);
	ColumnToRow(res_Reg_1, 4, function);
	ColumnToRow(res_Reg_2, 5, function);
	ColumnToRow(res_Reg_3, 6, function);
	ColumnToRow(res_Reg_4, 7, function);

}

void WRITE_Matrix(int row, int col, int W[][col], int reg, int copy){

	int s, t, i;
	if (copy == 1)		//write a 2D array, starts from the left-most PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(s/(4 * Tile_dim), t/(4 * Tile_dim), (s/4)%Tile_dim, (t/4)%Tile_dim, ((s*4+t)%4+s*4)%16, reg, W[s][t]);
			}
		}
	}
	else if(copy == 0)	//write a 1D array into last column PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(s/(4 * Tile_dim), Array_dim-1, (s/4)%Tile_dim, Tile_dim-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
	}
	else if(copy == -1)	//write a 1D array into first column PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(0, s/Array_dim, 0, (s/4)%Tile_dim, s%4, reg, W[s][t]);
			}
		}
	}
	else				//write a 1D array into first row, copies to the below PEs too
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(s/(4 * Tile_dim), Array_dim-1, (s/4)%Tile_dim, Tile_dim-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
		ColumnToRow(reg, reg, 0);

		int instruction = (7<<26) + (reg<<21) + (reg << 16) + (0 << 11);
		//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
		for( i = 0; i < copy-1; i++)
		{
			//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
			//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		}

	}

}

void WRITE_Matrix_Large(int row, int col, int block_row, int block_col, int row_blk_size, int col_blk_size, int W[][col], int reg, int copy, int block_dimension){


	int ss, tt, s, t, i;
	if (copy == 1)		//write a 2D array, starts from the left-most PEs
	{
		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for( t = col_blk_size * block_col; t < ((block_col + 1) * col_blk_size); t++)
			{
				ss = s - (row_blk_size * block_row);
				tt = t - (col_blk_size * block_col);
				WRITE_REG(ss/(4 * Tile_dim), tt/(4 * Tile_dim), (ss/4)%Tile_dim, (tt/4)%Tile_dim, ((ss*4+tt)%4+ss*4)%16, reg, W[s][t]);

			}
		}
	}
	else if(copy == 0)	//write a 1D array into last column PEs
	{
		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for( t = col_blk_size * block_col; t < col; t++)
			{
				ss = s - (row_blk_size * block_row);
				tt = t - (col_blk_size * block_col);
				WRITE_REG(ss/(4 * Tile_dim), Array_dim-1, (ss/4)%Tile_dim, Tile_dim-1, (ss*4+3)%16, reg, W[s][t]);

			}
		}
	}
	else if(copy == -1)	//write a 1D array into first column PEs
	{
		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for( t = col_blk_size * block_col; t < col; t++)
			{
				ss = s - (row_blk_size * block_row);
				tt = t - (col_blk_size * block_col);
				WRITE_REG(0, ss/Array_dim, 0, (ss/4)%Tile_dim, ss%4, reg, W[s][t]);
			}
		}
	}
	else				//write a 1D array into first row, copies to the below PEs too
	{
		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
		{
			for( t = col_blk_size * block_col; t < col; t++)
			{
				ss = s - (row_blk_size * block_row);
				tt = t - (col_blk_size * block_col);
				WRITE_REG(ss/(4 * Tile_dim), Array_dim-1, (ss/4)%Tile_dim, Tile_dim-1, (ss*4+3)%16, reg, W[s][t]);
			}
		}
		ColumnToRow(reg, reg, 0);

		int instruction = (7<<26) + (reg<<21) + (reg << 16) + (0 << 11);
		//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
		for( i = 0; i < copy-1; i++)
		{
			//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
			//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		}

	}

}

int toggleBit(int n, int k){

    return (n ^ (1 << (k - 1)));
}

void WRITE_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data){

	int base = reg*32;
	int i;
	for( i=base-2; i>=base-33; i-=2){

		unsigned int dia = ((data<<(31-i))>>31)<<PE;
		unsigned int dib = ((data<<(31-i-1))>>31)<<PE;
		unsigned int tmp, oldDia, oldDib, newDia, newDib;
		tmp = READ(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1);
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

		WRITE(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1, newDia, newDib);
	}
}

void WRITE(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB){

	int reg7_input = (Tile_i << 16) + Tile_j;
	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;
	int reg4_input = (DIA << 16) + DIB;

	//Tile_i = 0, Tile_j = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG7_OFFSET, reg7_input);

	//BRAM_i = 0, BRAM_j = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG2_OFFSET, reg2_input);

	//ADDRA = 32 , ADDRB = 64
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG3_OFFSET, reg3_input);

	//DIA = 0xFFFF, DIB = 0x1111
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG4_OFFSET, reg4_input);

	//external = 1, WEA = 1, WEB = 1
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x0000001D+LEN);

	//external = 1, WEA = 0, WEB = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG4_OFFSET, 0x00000000);

}

int READ(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB){

	int reg7_input = (Tile_i << 16) + Tile_j;
	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;
	
	int DO;

	//Tile_i = 0, Tile_j = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG7_OFFSET, reg7_input);

	//BRAM_i = 0, BRAM_j = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG2_OFFSET, reg2_input);

	//ADDRA = 32 , ADDRB = 64
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG3_OFFSET, reg3_input);

	//external = 1, WEA = 0, WEB = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000005+LEN);

	//read
	//int DO = ARP_WO_DFF_mReadReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG5_OFFSET);

	//external = 1, WEA = 0, WEB = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG3_OFFSET, 0x00000000);

	return DO;
}

int printReg(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg){

	int REG[32];
	int reg_out;
	int DO;
	reg = reg << 5;
	int i;
	for( i=0; i<16; i++){
		DO = READ (Tile_i, Tile_j, BRAM_i, BRAM_j, reg+i, reg+i+16);
		REG[i+16] = getbit(DO, PE);
		REG[i] = getbit(DO, PE+16);
	}

	reg_out = getTrans(REG);

	return reg_out;
}

void printRegFile(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int number_of_regs){

	int PE, i;
	for( PE=0; PE<16; PE++){
		for( i=number_of_regs-1; i>=0; i--)
		{
			printf("%08x \t", printReg(Tile_i, Tile_j, BRAM_i, BRAM_j, PE, i));
		}
		printf("\n\r");
	}

}

int execute(int opcode, int rd, int rs1, int rs2)
{
	int instruction = (opcode<<26) + (rd<<21) + (rs1 << 16) + (rs2 << 11);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);//start = 0, reset = 0
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
	//MB_Sleep(1);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	//MB_Sleep(1);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
	//MB_Sleep(1);
	//ARP_WO_DFF_mWriteReg(XPAR_ARP_WO_DFF_0_S00_AXI_BASEADDR, ARP_WO_DFF_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	//MB_Sleep(1);
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
		//arr[i] = readRAM(address+i);
		//cleanup_platform();
	}
}
