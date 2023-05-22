import numpy as np

inputsize = 65
hSize = 128
layer1Size = 128
layer2Size = 128

input = np.empty([65])
c0 = np.empty([128])
h0 = np.empty([128])
output = np.empty([65])
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
    global input, c0, h0
    input = np.ones(inputsize)
    c0 = np.ones(128)
    h0 = np.ones(128)

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

if __name__=="__main__":
    Populate()
    #lstmlayer 2
    i1 = Gate(x=input, Wi=Wii1, Bi=Bii1, h=h0, Wh=Whi1, Bh=Bhi1, act_func='sigmoid')
    f1 = Gate(x=input, Wi=Wif1, Bi=Bif1, h=h0, Wh=Whf1, Bh=Bhf1, act_func='sigmoid')
    g1 = Gate(x=input, Wi=Wig1, Bi=Big1, h=h0, Wh=Whg1, Bh=Bhg1, act_func='tanh')
    o1 = Gate(x=input, Wi=Wio1, Bi=Bio1, h=h0, Wh=Who1, Bh=Bho1, act_func='sigmoid')
    c1 = (f1*c0) + (i1*g1)
    h1 = o1*(np.tanh(c1))
    x1  = Dropout1.dot(h1)
    

    #lstmlayer 1
    i2 = Gate(x=x1, Wi=Wii2, Bi=Bii2, h=h1, Wh=Whi2, Bh=Bhi2, act_func='sigmoid')
    f2 = Gate(x=x1, Wi=Wif2, Bi=Bif2, h=h1, Wh=Whf2, Bh=Bhf2, act_func='sigmoid')
    g2 = Gate(x=x1, Wi=Wig2, Bi=Big2, h=h1, Wh=Whg2, Bh=Bhg2, act_func='tanh')
    o2 = Gate(x=x1, Wi=Wio2, Bi=Bio2, h=h1, Wh=Who2, Bh=Bho2, act_func='sigmoid')
    c2 = (f2*c1) + (i2*g2)
    h2 = o1*(np.tanh(c2))
    print(h2)
    #fully
