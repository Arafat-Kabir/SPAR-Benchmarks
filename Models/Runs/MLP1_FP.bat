ECHO "Starting MLP1 Run"
ECHO %~dp0
cd ../C_FP_Models
make
mlp1.exe

cd ../Pytorch_Models
python mlp1.py ../WeightsAndBiases/mlp1_fp.json
cd ../
python compare_outputs.py ./outputs/mlp_c_fp_output.txt ./outputs/mlp1_python_output.txt ./outputs/MLP1_Run_Output.txt
cd Runs