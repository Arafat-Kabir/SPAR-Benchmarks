ECHO "Starting MLP1 Run"
cd C_Models make
./mlp1.exe
cd ../Pytorch_Models
python mlp1.py
cd ../
python compare_outputs.py ./outputs/mlp_c_output.txt ./outputs/mlp1_python_output.txt ./outputs/MLP1_Run_Output.txt