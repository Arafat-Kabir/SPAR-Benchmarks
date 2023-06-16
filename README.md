# SPAR-Benchmarks
C_Models: Include floating point models for MLP and LSTM in C. Use register-based instructions. When run, the biases and weights are exported to JSON files.
Pytorch_Models: NOT Pytorch, just python floating point models. When given the arguement, the weights and biases are read from a json file as an argument.

Runs: These are bat files used to run the c models first, then the python models, and then finally compare their outputs.

outputs: This contains the final output for each model.