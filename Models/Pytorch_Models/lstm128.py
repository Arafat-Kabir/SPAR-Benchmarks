import numpy as np
import json
import sys
import os
inputsize = 65
hSize = 128
cSize = 128
layer1Size = 128
layer2Size = 128
outputsize = 65

input = np.empty([inputsize])
c_l1 = np.empty([cSize])
h_l1 = np.empty([hSize])
c_l2 = np.empty([cSize])
h_l2 = np.empty([hSize])
output = np.empty([outputsize])
#layer 1 weights
#for i1
Wii1 = np.empty([layer1Size, inputsize])
Bii1 = np.empty([layer1Size])
Whi1 = np.empty([layer1Size, hSize])
Bhi1 = np.empty([layer1Size])
#for f1
Wif1 = np.empty([layer1Size, inputsize])
Bif1 = np.empty([layer1Size])
Whf1 = np.empty([layer1Size, hSize])
Bhf1 = np.empty([layer1Size])

#for g1
Wig1 = np.empty([layer1Size, inputsize])
Big1 = np.empty([layer1Size])
Whg1 = np.empty([layer1Size, hSize])
Bhg1 = np.empty([layer1Size])

#for o1
Wio1 = np.empty([layer1Size, inputsize])
Bio1 = np.empty([layer1Size])
Who1 = np.empty([layer1Size, hSize])
Bho1 = np.empty([layer1Size])

#Dropout (I think this is just 1's and 0's since it is Bernoulli random variables)
Dropout1 = np.empty([inputsize, layer1Size])

#layer 2 weights
#for i2
Wii2 = np.empty([layer2Size, inputsize])
Bii2 = np.empty([layer2Size])
Whi2 = np.empty([layer2Size, hSize])
Bhi2 = np.empty([layer2Size])
#for f2
Wif2 = np.empty([layer2Size, inputsize])
Bif2 = np.empty([layer2Size])
Whf2 = np.empty([layer2Size, hSize])
Bhf2 = np.empty([layer2Size])
#for g2
Wig2 = np.empty([layer2Size, inputsize])
Big2 = np.empty([layer2Size])
Whg2 = np.empty([layer2Size, hSize])
Bhg2 = np.empty([layer2Size])
#for o2
Wio2 = np.empty([layer2Size, inputsize])
Bio2 = np.empty([layer2Size])
Who2 = np.empty([layer2Size, hSize])
Bho2 = np.empty([layer2Size])

#Fully Connected Layer
W_FC = np.empty([outputsize, layer2Size])
B_FC = np.empty([outputsize])


def Sigmoid(x):
    return 1/(1 + np.exp(-x))

def Gate(x, Wi, Bi, h, Wh, Bh, act_func):
    Wix = Wi.dot(x)
    Wix_b = Wix + Bi
    Whh = Wh.dot(h)
    Whh_b = Whh + Bh
    Result = Wix_b + Whh_b
    if(act_func.lower() == "sigmoid"):
       Sigmoid_Result = Sigmoid(Result)
       return Sigmoid_Result
    elif(act_func.lower() == "tanh"):
        Tanh_Result = np.tanh(Result)
        return Tanh_Result
    else:
       return Result

def Populate():
    global input, c_l1, h_l1
    input = np.ones(inputsize)
    #leaving cell and hidden states as 0 for now
    # c0_l1 = np.ones(128)
    # h0_l1 = np.ones(128)

    global Wii1, Bii1, Whi1, Bhi1
    Wii1 = np.ones([layer1Size, inputsize])
    Bii1 = np.ones([layer1Size])
    Whi1 = np.ones([layer1Size, hSize])
    Bhi1 = np.ones([layer1Size])
    #for f1
    global Wif1, Bif1, Whf1, Bhf1
    Wif1 = np.ones([layer1Size, inputsize])
    Bif1 = np.ones([layer1Size])
    Whf1 = np.ones([layer1Size, hSize])
    Bhf1 = np.ones([layer1Size])

    #for g1
    global Wig1, Big1, Whg1, Bhg1
    Wig1 = np.ones([layer1Size, inputsize])
    Big1 = np.ones([layer1Size])
    Whg1 = np.ones([layer1Size, hSize])
    Bhg1 = np.ones([layer1Size])

    #for o1
    global Wio1, Bio1, Who1, Bho1
    Wio1 = np.ones([layer1Size, inputsize])
    Bio1 = np.ones([layer1Size])
    Who1 = np.ones([layer1Size, hSize])
    Bho1 = np.ones([layer1Size])

    #Dropout (I think this is just 1's and 0's since it is Bernoulli random variables)
    global Dropout1
    Dropout1 = np.ones([inputsize, layer1Size])

    #layer 2 weights
    #for i2
    global Wii2, Bii2, Whi2, Bhi2
    Wii2 = np.ones([layer2Size, inputsize])
    Bii2 = np.ones([layer2Size])
    Whi2 = np.ones([layer2Size, hSize])
    Bhi2 = np.ones([layer2Size])
    #for f2
    global Wif2, Bif2, Whf2, Bhf2
    Wif2 = np.ones([layer2Size, inputsize])
    Bif2 = np.ones([layer2Size])
    Whf2 = np.ones([layer2Size, hSize])
    Bhf2 = np.ones([layer2Size])
    #for g2
    global Wig2, Big2, Whg2, Bhg2
    Wig2 = np.ones([layer2Size, inputsize])
    Big2 = np.ones([layer2Size])
    Whg2 = np.ones([layer2Size, hSize])
    Bhg2 = np.ones([layer2Size])
    #for o2
    global Wio2, Bio2, Who2, Bho2
    Wio2 = np.ones([layer2Size, inputsize])
    Bio2 = np.ones([layer2Size])
    Who2 = np.ones([layer2Size, hSize])
    Bho2 = np.ones([layer2Size])

    #Fully Connected Layer
    global W_FC, B_FC
    W_FC = np.ones([outputsize, layer2Size])
    B_FC = np.ones([outputsize])

def LoadWeights(file_name):
    with open(file_name) as json_file:
        data = json.load(json_file)
        global inputsize, hSize, cSize, layer1Size, layer2Size, outputsize
        inputsize = data['X_Size']
        hSize = data['H_Size']
        cSize = data['C_Size']
        layer1Size = data['W1_Rows']
        layer2Size = data['W2_Rows']
        outputsize = data['Output_Size']
        global input, h_l1, c_l1
        input = np.array(data['Input'])
        h_l1 = np.array(data['H0'])
        c_l1 = np.array(data['C0'])
        global Wii1, Bii1, Whi1, Bhi1
        Wii1 = np.array(data['Wii1']).reshape([layer1Size, inputsize])
        Bii1 = np.array(data['Bii1'])
        Whi1 = np.array(data['Whi1']).reshape([layer1Size, hSize])
        Bhi1 = np.array(data['Bhi1'])
        global Wif1, Bif1, Whf1, Bhf1
        Wif1 = np.array(data['Wif1']).reshape([layer1Size, inputsize])
        Bif1 = np.array(data['Bif1'])
        Whf1 = np.array(data['Whf1']).reshape([layer1Size, hSize])
        Bhf1 = np.array(data['Bhf1'])
        global Wig1, Big1, Whg1, Bhg1
        Wig1 = np.array(data['Wig1']).reshape([layer1Size, inputsize])
        Big1 = np.array(data['Big1'])
        Whg1 = np.array(data['Whg1']).reshape([layer1Size,hSize])
        Bhg1 = np.array(data['Bhg1'])
        global Wio1, Bio1, Who1, Bho1
        Wio1 = np.array(data['Wio1']).reshape([layer1Size,inputsize])
        Bio1 = np.array(data['Bio1'])
        Who1 = np.array(data['Who1']).reshape([layer1Size, hSize])
        Bho1 = np.array(data['Bho1'])
        global Dropout1
        Dropout1 = np.array(data['Dropout1']).reshape([inputsize, layer1Size])

        global Wii2, Bii2, Whi2, Bhi2
        Wii2 = np.array(data['Wii2']).reshape([layer2Size, inputsize])
        Bii2 = np.array(data['Bii2'])
        Whi2 = np.array(data['Whi2']).reshape([layer2Size, hSize])
        Bhi2 = np.array(data['Bhi2'])
        global Wif2, Bif2, Whf2, Bhf2
        Wif2 = np.array(data['Wif2']).reshape([layer2Size, inputsize])
        Bif2 = np.array(data['Bif2'])
        Whf2 = np.array(data['Whf2']).reshape([layer2Size, hSize])
        Bhf2 = np.array(data['Bhf2'])
        global Wig2, Big2, Whg2, Bhg2
        Wig2 = np.array(data['Wig2']).reshape([layer2Size, inputsize])
        Big2 = np.array(data['Big2'])
        Whg2 = np.array(data['Whg2']).reshape([layer2Size, hSize])
        Bhg2 = np.array(data['Bhg2'])
        global Wio2, Bio2, Who2, Bho2
        Wio2 = np.array(data['Wio2']).reshape([layer2Size, inputsize])
        Bio2 = np.array(data['Bio2'])
        Who2 = np.array(data['Who2']).reshape([layer2Size, hSize])
        Bho2 = np.array(data['Bho2'])
        global W_FC, B_FC
        W_FC = np.array(data['WFC']).reshape([outputsize, layer2Size])
        B_FC = np.array(data['BFC'])
        return

if __name__=="__main__":
    Populate()
    if(len(sys.argv)>=2):
        if(os.path.exists(sys.argv[1])):
            print("found file"+ sys.argv[1])
            LoadWeights(sys.argv[1])
        else:
            print("Cannot Find Specified File")
    #lstmlayer 1
    i1 = Gate(x=input, Wi=Wii1, Bi=Bii1, h=h_l1, Wh=Whi1, Bh=Bhi1, act_func='sigmoid')
    f1 = Gate(x=input, Wi=Wif1, Bi=Bif1, h=h_l1, Wh=Whf1, Bh=Bhf1, act_func='sigmoid')
    g1 = Gate(x=input, Wi=Wig1, Bi=Big1, h=h_l1, Wh=Whg1, Bh=Bhg1, act_func='tanh')
    o1 = Gate(x=input, Wi=Wio1, Bi=Bio1, h=h_l1, Wh=Who1, Bh=Bho1, act_func='sigmoid')
    c_l1 = (f1*c_l1) + (i1*g1) #update the cell state
    h_l1 = o1*(np.tanh(c_l1)) #update the hidden state
    x1  = Dropout1.dot(h_l1) #predict x to feed the next layer

    #lstmlayer 2
    i2 = Gate(x=x1, Wi=Wii2, Bi=Bii2, h=h1, Wh=Whi2, Bh=Bhi2, act_func='sigmoid')
    f2 = Gate(x=x1, Wi=Wif2, Bi=Bif2, h=h1, Wh=Whf2, Bh=Bhf2, act_func='sigmoid')
    g2 = Gate(x=x1, Wi=Wig2, Bi=Big2, h=h1, Wh=Whg2, Bh=Bhg2, act_func='tanh')
    o2 = Gate(x=x1, Wi=Wio2, Bi=Bio2, h=h1, Wh=Who2, Bh=Bho2, act_func='sigmoid')
    c_l2 = (f2*c_l2) + (i2*g2) #update the cell state for layer 2
    h_l2 = o1*(np.tanh(c_l2)) #update the hidden state for layer 2
    #fully connected layer using just the layer 2 hidden state for now
    WfcH = W_FC.dot(h_l2)
    # print(WfcH)
    WfcH_b = WfcH + B_FC
    print(B_FC)
    fout = open("../outputs/lstm128_python_output.txt", "w")
    for i in range(outputsize):
        fout.write("{:.4f},\n".format(WfcH_b[i]))

