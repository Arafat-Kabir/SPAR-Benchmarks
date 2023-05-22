import sys
import os

if len(sys.argv) < 3:
    print("Error: Not Given 2 Files to Compare")
    exit()

print("File List: ", str(sys.argv))
if(os.path.exists(sys.argv[1])):
    print("file 1 exists")
if(os.path.exists(sys.argv[2])):
    print("file 2 exists")

file1 = open(sys.argv[1], "r")
file2 = open(sys.argv[2], "r")
if(len(sys.argv) >=4 ):
    output = open(sys.argv[3], "w")
else:
    output = open("outputs_diff.txt", "w")

values1 = file1.read().split(',\n')
values2 = file2.read().split(',\n')

length = min(len(values1), len(values2))
values3 = []
difference_squared= 0.0
errors = 0
for i in range(length):
    try:
        temp = float(values1[i])-float(values2[i])
        values3.append(temp)
        difference_squared += (temp*temp)
    except:
        values3.append("Error")
        errors+=1
# percent_dif_sum = percent_dif_sum/100.0
print("diff^2: {:.4f}, errors: {:d}".format(difference_squared, errors))
output.write("diff^2: {:f}, errors: {:d}".format(difference_squared, errors))
for i in range(length):
    output.write(str(values3[i]))
    output.write('\n')