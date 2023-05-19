import numpy as np

def ReLU(x):
    return x*(x>=0)

inputsize = 78
m1 = 10
m2 = 10
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

#layer1
print(weight1)
print(input)
wx1 = weight1.dot(input)
print(wx1)
wx1_b = wx1+bias1
print(wx1_b)
Relu_wx1_b = ReLU(wx1_b)

#layer2
wx2 = weight2.dot(Relu_wx1_b)
wx2_b = wx2+bias2
Relu_wx2_b = ReLU(wx2_b)
