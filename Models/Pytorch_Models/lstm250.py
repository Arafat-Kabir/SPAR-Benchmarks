import numpy as np
import json
import sys
import os
inputsize = 61
layer1Size = 250
layer2Size = 250
layer3Size = 250
outputsize = 39

input = np.empty([inputsize])
c_l1 = np.empty([layer1Size])
h_l1 = np.empty([layer1Size])
c_l2 = np.empty([layer2Size])
h_l2 = np.empty([layer2Size])
c_l3 = np.empty([layer3Size])
h_l3 = np.empty([layer3Size])

output = np.empty([outputsize])
#layer 1 weights
#for i1
Wii1 = np.empty([layer1Size, inputsize])
Bii1 = np.empty([layer1Size])
Whi1 = np.empty([layer1Size, layer1Size])
Bhi1 = np.empty([layer1Size])
#for f1
Wif1 = np.empty([layer1Size, inputsize])
Bif1 = np.empty([layer1Size])
Whf1 = np.empty([layer1Size, layer1Size])
Bhf1 = np.empty([layer1Size])

#for g1
Wig1 = np.empty([layer1Size, inputsize])
Big1 = np.empty([layer1Size])
Whg1 = np.empty([layer1Size, layer1Size])
Bhg1 = np.empty([layer1Size])

#for o1
Wio1 = np.empty([layer1Size, inputsize])
Bio1 = np.empty([layer1Size])
Who1 = np.empty([layer1Size, layer1Size])
Bho1 = np.empty([layer1Size])

#Dropout (I think this is just 1's and 0's since it is Bernoulli random variables)
Dropout1 = np.empty([inputsize, layer1Size])

#layer 2 weights
#for i2
Wii2 = np.empty([layer2Size, inputsize])
Bii2 = np.empty([layer2Size])
Whi2 = np.empty([layer2Size, layer2Size])
Bhi2 = np.empty([layer2Size])
#for f2
Wif2 = np.empty([layer2Size, inputsize])
Bif2 = np.empty([layer2Size])
Whf2 = np.empty([layer2Size, layer2Size])
Bhf2 = np.empty([layer2Size])
#for g2
Wig2 = np.empty([layer2Size, inputsize])
Big2 = np.empty([layer2Size])
Whg2 = np.empty([layer2Size, layer2Size])
Bhg2 = np.empty([layer2Size])
#for o2
Wio2 = np.empty([layer2Size, inputsize])
Bio2 = np.empty([layer2Size])
Who2 = np.empty([layer2Size, layer2Size])
Bho2 = np.empty([layer2Size])

#Dropout (I think this is just 1's and 0's since it is Bernoulli random variables)
Dropout2 = np.empty([inputsize, layer2Size])

#layer 3 weights
#for i3
Wii3 = np.empty([layer3Size, inputsize])
Bii3 = np.empty([layer3Size])
Whi3 = np.empty([layer3Size, layer3Size])
Bhi3 = np.empty([layer3Size])
#for f3
Wif3 = np.empty([layer3Size, inputsize])
Bif3 = np.empty([layer3Size])
Whf3 = np.empty([layer3Size, layer3Size])
Bhf3 = np.empty([layer3Size])
#for g3
Wig3 = np.empty([layer3Size, inputsize])
Big3 = np.empty([layer3Size])
Whg3 = np.empty([layer3Size, layer3Size])
Bhg3 = np.empty([layer3Size])
#for o3
Wio3 = np.empty([layer3Size, inputsize])
Bio3 = np.empty([layer3Size])
Who3 = np.empty([layer3Size, layer3Size])
Bho3 = np.empty([layer3Size])

#Fully Connected Layer
WFC = np.empty([outputsize,layer3Size])
BFC = np.empty([outputsize])

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
    
def run_inference_lstm250():
    #load in data
    
    #layer1
    i1 = Gate(x=input, Wi=Wii1, Bi=Bii1, h=h_l1, Wh=Whi1, Bh=Bhi1, act_func='sigmoid')
    f1 = Gate(x=input, Wi=Wif1, Bi=Bif1, h=h_l1, Wh=Whf1, Bh=Bhf1, act_func='sigmoid')
    g1 = Gate(x=input, Wi=Wig1, Bi=Big1, h=h_l1, Wh=Whg1, Bh=Bhg1, act_func='tanh')
    o1 = Gate(x=input, Wi=Wio1, Bi=Bio1, h=h_l1, Wh=Who1, Bh=Bho1, act_func='sigmoid')
    c_l1 = (f1*c_l1) + (i1*g1) #update the cell state
    h_l1 = o1*(np.tanh(c_l1)) #update the hidden state
    x1  = Dropout1.dot(h_l1) #predict x to feed the next layer

    #layer2
    i2 = Gate(x=input, Wi=Wii2, Bi=Bii2, h=h_l2, Wh=Whi2, Bh=Bhi2, act_func='sigmoid')
    f2 = Gate(x=input, Wi=Wif2, Bi=Bif2, h=h_l2, Wh=Whf2, Bh=Bhf2, act_func='sigmoid')
    g2 = Gate(x=input, Wi=Wig2, Bi=Big2, h=h_l2, Wh=Whg2, Bh=Bhg2, act_func='tanh')
    o2 = Gate(x=input, Wi=Wio2, Bi=Bio2, h=h_l2, Wh=Who2, Bh=Bho2, act_func='sigmoid')
    c_l2 = (f2*c_l2) + (i2*g2) #update the cell state
    h_l2 = o2*(np.tanh(c_l2)) #update the hidden state
    x2  = Dropout2.dot(h_l2) #predict x to feed the next layer

    #layer3
    i3 = Gate(x=input, Wi=Wii3, Bi=Bii3, h=h_l3, Wh=Whi3, Bh=Bhi3, act_func='sigmoid')
    f3 = Gate(x=input, Wi=Wif3, Bi=Bif3, h=h_l3, Wh=Whf3, Bh=Bhf3, act_func='sigmoid')
    g3 = Gate(x=input, Wi=Wig3, Bi=Big3, h=h_l3, Wh=Whg3, Bh=Bhg3, act_func='tanh')
    o3 = Gate(x=input, Wi=Wio3, Bi=Bio3, h=h_l3, Wh=Who3, Bh=Bho3, act_func='sigmoid')
    c_l3 = (f3*c_l3) + (i3*g3) #update the cell state
    h_l3 = o3*(np.tanh(c_l3)) #update the hidden state

    #fully connected layer
    WfcH = WFC.dot(h_l3)
    WfcH_b = WfcH + BFC
    print(WfcH_b)