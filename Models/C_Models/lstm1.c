void lstm65_128_128_65(){
    int x_size = 65;
    int H0_size = 128;
    int C0_size = 128;
    int W1_Rows = 128;
    int W2_Rows = 128;
    int WFC_Rows = 65;

    int X0[x_size];
    int H0[H0_size];
    int C0[C0_size];
    //H1, C1, etc are calculated at runtime
    //Wi is for x, Wh is for H
    //Wii is for finding i, Wif is for finding f, etc

    //----layer 1 weights and biases----//
    //weights and biases for i1 (i of layer 1)
    int Wii1[x_size][W1_Rows];
    int Bii[W1_Rows];
    int Whi1[H0_size][W1_Rows];
    int Bhi[W1_Rows];
    //f1
    int Wif1[x_size][W1_Rows];
    int Bif1[W1_Rows];
    int Whi1[H0_size][W1_Rows];
    int Bhf1[W1_Rows];
    //g1
    int Wig1[x_size][W1_Rows];
    int Big1[W1_Rows];
    int Whg1[H0_size][W1_Rows];
    int Bhg1[W1_Rows];
    //o1
    int Wio1[x_size][W1_Rows];
    int Bio1[W1_Rows];
    int Who1[H0_size][W1_Rows];
    int Bho1[W1_Rows];

    //conversion variable (turns Ht-1 into predicted x for next input)
    int Bernoulli1[W1_Rows][x_size];

    //----layer 2 weights and biases----//
    //i2
    int Wii2[x_size][W2_Rows];
    int Bii[W2_Rows];
    int Whi2[H0_size][W2_Rows];
    int Bhi[W2_Rows];
    //f2
    int Wif2[x_size][W2_Rows];
    int Bif2[W2_Rows];
    int Whi2[H0_size][W2_Rows];
    int Bhf2[W2_Rows];
    //g2
    int Wig2[x_size][W2_Rows];
    int Big2[W2_Rows];
    int Whg2[H0_size][W2_Rows];
    int Bhg2[W2_Rows];
    //o2
    int Wio2[x_size][W2_Rows];
    int Bio2[W2_Rows];
    int Who2[H0_size][W2_Rows];
    int Bho2[W2_Rows];

    //----Fully Connected Layer----//
    int WFC[W2_Rows][WFC_Rows];
    int BFC[WFC_Rows];
}