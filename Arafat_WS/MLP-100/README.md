# Development and Training

## TODO

- [x] Notebook for PyTorch model design and training
    - [x] Train on whole dataset
    - [x] Export as PyTorch model
    - [ ] Export test dataset as dictionary with torch.save()
- [ ] Notebook for Numpy model design and validation
    - [ ] Test on the test dataset exported by training notebook
    - [ ] Export model weights and dataset as sqlite3 databases for C implementations
- [ ] Notebook for experimenting with fixed-point precisions using Numpy
    - [ ] Experiment to figure out a precision that works for this model
    - [ ] Export model weights and dataset as sqlite3 databases for C fixed-poing implementation
- [ ] Implement the model in C from the exported databases
    - [ ] Follow C driver architecture ... (requires more thoughts)
    - [ ] Write the model using the CPU driver
