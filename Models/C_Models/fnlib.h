// header guard



typedef int reg;
typedef float* mat;
typedef float* vec;
typedef int acttype;

void load_m(reg a, void * b, int rows, int cols);
void load_v(reg a, vec b, int size);
void load_v_t(reg a, vec b, int size);
void printreg(reg a);
void printreg_v(reg a);
void printreg_to_file(reg a, int row, int col, char *filename);
void e_mul_mv(reg m, reg v, int rowm, int colm, reg result);
void acc_col(reg m, int rowm, int colm, reg temp, reg result);
void add(reg a, reg b, reg des);
void ReLU(reg a, reg des);
void activation(reg a, acttype act);
void rotate(reg a);
void store(reg a, vec b);
