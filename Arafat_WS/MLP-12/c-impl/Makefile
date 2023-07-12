

default: .list


.list:
	@echo Select a target
	@egrep '^(\w|\.)+:' Makefile | cut -f1 -d: | sed 's/^/    /'


mnist_dataset.o: mnist_dataset.h  mnist_dataset.c
	gcc  -std=c99  -c mnist_dataset.c -o mnist_dataset.o


mlp12_params.o: mlp12_params.c  mlp12_params.h
	gcc  -std=c99  -c mlp12_params.c  -o mlp12_params.o


driver.o: driver-main.c mnist_dataset.h mlp12_params.h utils.h
	gcc  -std=c99  -c driver-main.c  -o driver.o


utils.o: utils.c utils.h
	gcc  -std=c99  -c utils.c -o utils.o


driver.out: driver.o mlp12_params.o mnist_dataset.o utils.o
	gcc  driver.o mlp12_params.o mnist_dataset.o utils.o  -o driver.out


run: driver.out
	@echo -e '\n\n ---- [ Running driver.out ] ----'
	@./driver.out


clean:
	rm -f *.o *.out
