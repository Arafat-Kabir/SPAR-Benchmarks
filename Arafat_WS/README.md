# Developement Workspace for Arafat

## TODO

### Overall Goals

- [ ] Fix network architectures
- [ ] Train on whole datasets
- [ ] Export as Numpy models
- [ ] Fix the fixed-point precisions for each model
- [ ] Export for C model: sqlite3 database
- [ ] Design C-driver architectures: CPU, SPAR, Emulator
- [ ] Implement the CPU driver
- [ ] Verify accuracy on floating point and fixed-point implmentations
- [ ] Freeze the C implementations of the models


### Network architectures

| Architecture | Description 
|--------------|-------------
| MLP-12       | Single layer MLP for MNIST (784-FC:12-10).
| MLP-100      | 2-layer MLP for MNIST (784-FC:100-FC:100-10).
| LSTM-2L-250H | 2-layer LSTM for TIMIT phoneme recognition (61-LSTM:250-LSTM:250-FC-39)
| LSTM-xL-nH   | x-layer LSTM for TIMIT phoneme recognition (TBD)
