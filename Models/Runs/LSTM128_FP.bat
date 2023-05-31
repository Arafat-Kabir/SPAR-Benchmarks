ECHO "Starting LSTm128 Run"
ECHO %~dp0
cd ../C_FP_Models
del mlp1.exe
make
mlp1.exe

cd ../Pytorch_Models
python lstm128.py ../WeightsAndBiases/lstm128_fp.json
cd ../
python compare_outputs.py  ./outputs/lstm128_c_output.txt ./outputs/lstm128_python_output.txt ./outputs/lstm128_run.output.txt
cd Runs