#include "mlp1.h"
#include "lstm128.h"
#include "fplib.h"
#include <stdio.h>

int main() {
    // void *p = 0;
    // initialize(p);
    // run_inference();
    // something else: softmax
    // run_inference_lstm128();
    fpvar a = {100, 3, 2};
    fpvar b = {-200, 3, 2};
    fpvar c = fp_add(a, b);
    printf("%llX\n", c);
    printf("%lld\n", c);
    return 0;
}
