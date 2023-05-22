import numpy as np
import json
import sys
import os

inputsize = 784
m1 = 100
m2 = 100
#[row, columns]
input = np.empty(inputsize, dtype=float)
weight1 = np.empty([m1, inputsize], dtype=float)
bias1 = np.empty(m1, dtype=float)
weight2 = np.empty([m2, m1], dtype=float)
bias2 = np.empty(m2, dtype=float)

for i in range(m1):
    for j in range(inputsize):
        weight1[i,j] = 1.0*i
for i in range(inputsize):
    input[i] = 1.0
for i in range(m1):
    bias1[i]= -10.0
for i in range(m2):
    for j in range(m1):
        weight2[i,j] = i*j
for i in range(m2):
    bias2[i] = 50.0

def ReLU(x):
    return x*(x>=0)

def PopulateData(file_name):
    with open(file_name) as json_file:
        data = json.load(json_file)
        print(data.keys())
        global inputsize
        inputsize = int(data['input_size'])
        global m1
        m1 = int(data['weight1_row'])
        global m2
        m2 = int(data['weight2_row'])
        global input
        global weight1
        global bias1
        global weight2
        global bias2
        input = np.array(data['input'])
        weight1 = np.array(data['weight1']).reshape([m1, inputsize])
        bias1 = np.array(data['bias1'])
        weight2 = np.array(data['weight2']).reshape([m2, m1])
        bias2 = np.array(data['bias2'])


if __name__=="__main__":
    if(len(sys.argv)>=2):
        if(os.path.exists(sys.argv[1])):
            PopulateData(sys.argv[1])
        else:
            print("Cannot Find Specified File")
        

    #layer1
    # print(weight1)
    wx1 = weight1.dot(input)
    wx1_b = wx1+bias1
    Relu_wx1_b = ReLU(wx1_b)

    #layer2
    wx2 = weight2.dot(Relu_wx1_b)
    wx2_b = wx2+bias2
    Relu_wx2_b = ReLU(wx2_b)

    #output
    fout = open("../outputs/mlp1_python_output.txt", "w")
    for i in range(m2):
        fout.write("{:.4f},\n".format(Relu_wx2_b[i]))