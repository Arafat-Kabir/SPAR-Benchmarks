from AK_FixedPoint import *


# Test Add functions
def test_add():
    print('Testing fxp_add():')
    total_width = 10
    frac_width = 5
    stat = fxp_Status(0,0,0,0,0,0)
    num1 = fxp_ctor(total_width, frac_width,  [10, 12, -3, 12, -10, 10])
    num2 = fxp_ctor(total_width, frac_width, [1, 2,  3, -6, -10, 10])
    tnum = np.array([2., 4., 0., 6., 12., -12.])
    num3 = fxp_add(num1, num2, stat)
    fxp_printInfo(num3)
    fxp_printData(num3)
    fxp_printValue(num3)
    result = (tnum == fxp_getAsFloat(num3))
    assert result.any(), "Result Mismatch"

    print(stat)
    assert stat.overflow_count==2, "Status Error"
    assert stat.overflow==True, "Status Error"




# Test Sub functions
def test_sub():
    print('Testing fxp_sub():')
    total_width = 10
    frac_width = 5
    stat = fxp_Status(0,0,0,0,0,0)
    num1 = fxp_ctor(total_width, frac_width, [10, 12, -3, 12, -10,  10, -8, -8])
    num2 = fxp_ctor(total_width, frac_width, [ 1,  2,  3, -6, -10, -10,  8,  9])
    tnum = np.array([9., 10., -6., -14., 0., -12., -16., 15.])
    num3 = fxp_sub(num1, num2, stat)
    fxp_printInfo(num3)
    fxp_printData(num3)
    fxp_printValue(num3)
    result = (tnum == fxp_getAsFloat(num3))
    assert result.any(), "Result Mismatch"

    print(stat)
    assert stat.overflow_count==3, "Status Error"
    assert stat.overflow==True, "Status Error"




# Test Mult functions
def test_mult():
    print('Testing Mult:')
    total_width = 10
    frac_width = 5
    stat = fxp_Status(0,0,0,0,0,0)
    num1 = fxp_ctor(total_width, frac_width, [2.4, 3.5, -3.1, 12, -16, -16])    # -16 x -1 overflows
    num2 = fxp_ctor(total_width, frac_width, [  1,   2,    3, -1,  -1,   1])    # -16 x 1 does not overflow
    tnum = np.array([  2.375, 7., -9.28125, -12., -16, -16])
    num3 = fxp_mult(num1, num2, stat)
    fxp_printInfo(num3)
    fxp_printData(num3)
    fxp_printValue(num3)
    fxp_printBinstr(num3)
    result = (tnum == fxp_getAsFloat(num3))
    assert result.all(), "Result Mismatch"

    print(stat)
    assert stat.overflow_count==1, "Status Error"
    assert stat.overflow==True, "Status Error"



# Count to check wrapping behavior
def test_wrap():
    print('Testing wrapping behavior:')
    total_width = 8
    frac_width = 4
    num_count = fxp_ctor(total_width, frac_width, 0)
    num_one = fxp_ctor(total_width, frac_width, 1)
    check_vals = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, -8.0, -7.0, -6.0, -5.0, -4.0, -3.0, -2.0, -1.0, 0.0]

    whole_width = total_width - frac_width
    for i in range(2**whole_width):
        num_count = fxp_add(num_count, num_one)
        fxp_printValue(num_count)
        assert fxp_getAsFloat(num_count) == check_vals[i], f"Expected: {check_vals[i]}   Got: {num_count._data}"




# Check fraction precision
def test_frac_precision():
    print('Testing fraction precision:')
    total_width = 6
    frac_width = 3
    scale = 2**frac_width
    num_count = fxp_ctor(total_width, frac_width, [1])
    num_frac = fxp_ctor(total_width, frac_width, [1/scale])
    check_vals = [9, 10, 11, 12, 13, 14, 15, 16]

    for i in range(2**frac_width):
        num_count = fxp_add(num_count, num_frac)
        fxp_printData(num_count)
        fxp_printValue(num_count)
        fxp_printBinstr(num_count)
        print('')
        assert num_count._data == check_vals[i], f"Expected: {check_vals[i]}   Got: {num_count._data}"



# Run all tests
def main():
    test_add();  print('\n')
    test_sub();  print('\n')
    test_mult();  print('\n')
    test_wrap();  print('\n')
    test_frac_precision();  print('\n')


if __name__=='__main__':
    main()
