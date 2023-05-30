import numpy as np
import json
import sys
import os

inputsize = 784
m1 = 100
m2 = 100
output = 10
#[row, columns]
input = np.empty(inputsize, dtype=float)
weight1 = np.empty([m1, inputsize], dtype=float)
bias1 = np.empty(m1, dtype=float)
weight2 = np.empty([m2, m1], dtype=float)
bias2 = np.empty(m2, dtype=float)
weight3 = np.empty([output, m2])
bias3 = np.empty(output)

for i in range(m1):
    for j in range(inputsize):
        weight1[i,j] = 1.0
for i in range(inputsize):
    input[i] = 1.0
for i in range(m1):
    bias1[i]= 1#-10.0
for i in range(m2):
    for j in range(m1):
        weight2[i,j] = 1.0
for i in range(m2):
    bias2[i] = 1.0
for i in range(output):
    for j in range(m2):
        weight3[i,j] = 1.0
for i in range(output):
    bias3[i] = 1.0

def ReLU(x):
    return x*(x>=0)


def PopulateData(file_name):
    global inputsize
    global m1
    global m2
    global input
    global weight1
    global bias1
    global weight2
    global bias2
    global weight3, bias3
    if 'csv' in file_name:
        print('csv files')
        input = np.loadtxt('../WeightsAndBiases/x.csv', delimiter=",")
        #layer1
        w1 = np.loadtxt('../WeightsAndBiases/w1.csv', delimiter=",")
        w1 = w1.reshape([m1, inputsize])
        bias1 =  np.loadtxt('../WeightsAndBiases/b1.csv', delimiter=",")
        #layer2
        w2 = np.loadtxt('../WeightsAndBiases/w2.csv', delimiter=",")
        w2 = w2.reshape([m2, m1])
        bias2 =  np.loadtxt('../WeightsAndBiases/b2.csv', delimiter=",")
        #layer3
        w3 = np.loadtxt('../WeightsAndBiases/w3.csv', delimiter=",")
        w3 = w3.reshape([output, m2])
        bias3 =  np.loadtxt('../WeightsAndBiases/b3.csv', delimiter=",")
    else:
        with open(file_name) as json_file:
            data = json.load(json_file)
            print(data.keys())
            inputsize = int(data['input_size'])
            m1 = int(data['weight1_row'])
            m2 = int(data['weight2_row'])
            input = np.array(data['input'])
            weight1 = np.array(data['weight1']).reshape([m1, inputsize])
            bias1 = np.array(data['bias1'])
            weight2 = np.array(data['weight2']).reshape([m2, m1])
            bias2 = np.array(data['bias2'])
            weight3 = np.array(data['weight3']).reshape([output, m2])
            bias3 = np.array(data['bias3']).reshape([output])


if __name__=="__main__":
    if(len(sys.argv)>=2):
        if(os.path.exists(sys.argv[1]) or sys.argv[1]=='csv'):
            PopulateData(sys.argv[1])
        else:
            print("Cannot Find Specified File")
        

    #layer1
    wx1 = weight1.dot(input)#close here
    wx1_b = wx1+bias1#seems good here
    Relu_wx1_b = ReLU(wx1_b)#seems good here

    #layer2
    wx2 = weight2.dot(Relu_wx1_b)#slightly off
    wx2_b = wx2+bias2
    Relu_wx2_b = ReLU(wx2_b)#slightly diff. Still close

    #output layer
    wx3 = weight3.dot(Relu_wx2_b)
    # print(weight3)
    # print(wx3)
    print(bias3)
    wx3_b = wx3 + bias3 #needs a softmax implementation after this. Should still work.
    # Relu_wx3_b = ReLU(wx3_b)
    print(wx3_b)

    fout = open("../outputs/mlp1_python_output.txt", "w")
    for i in range(output):
        fout.write("{:.4f},\n".format(wx3_b[i]))