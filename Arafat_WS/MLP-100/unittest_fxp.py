from AK_FixedPoint import *


# Testing fxp_ctor
print('fxp_ctor() tests:')

inp_vec = [ 8,  9, 4, 2, 31, -3, -9]
out_vec = [-8, -7, 4, 2, -1, -3, 7]        # 8 -> -8, 9 -> -7, 31 -> -1
fxp_tnum, stat = fxp_ctor(8, 4, inp_vec)  
print(inp_vec)
fxp_printValue(fxp_tnum)
print(stat)
#print(fxp_getAsBin(fxp_tnum))
result = (out_vec == fxp_getAsFloat(fxp_tnum))
assert result.all(), "Problem with fxp_ctor"

print('')
inp_vec = [ 16,  18, -4, -32, 32, -10]
out_vec = [-16, -14, -4, 0, 0, -10]
fxp_tnum, stat = fxp_ctor(10, 5, inp_vec)  
print(stat)
print(inp_vec)
fxp_printValue(fxp_tnum)
#print(fxp_getAsBin(fxp_tnum))
result = (out_vec == fxp_getAsFloat(fxp_tnum))
assert result.all(), "Problem with fxp_ctor()"


# Testing fxp_getSignBits()
print('\n\nfxp_getSignBits() tests:')

inp_vec = [ -2, -4, -5, 4, 5, 7]
out_vec = [1,1,1,0,0,0]
fxp_tnum, stat = fxp_ctor(8, 4, inp_vec)
print(stat)
sign_bits = fxp_getSignBits(fxp_tnum)
result = (sign_bits == out_vec)
print(out_vec)
print(sign_bits)
assert result.all(), "Problem with fxp_getSignBits()"


print('')
inp_vec = [ -10, 14, -5, 14, -5, 7]
out_vec = [1, 0, 1, 0, 1, 0]
fxp_tnum, stat = fxp_ctor(10, 5, inp_vec)
print(stat)
sign_bits = fxp_getSignBits(fxp_tnum)
result = (sign_bits == out_vec)
print(out_vec)
print(sign_bits)
assert result.all(), "Problem with fxp_getSignBits()"



# Test fxp_computeStatus()
print('\n\nfxp_computeStatus tests:')

inp_vec = [ 8,  9, 4, 2, 31, -3, -9]
out_vec = [-8, -7, 4, 2, -1, -3, 7]        # 8 -> -8, 9 -> -7, 31 -> -1

scale = 2**4
before = np.array(inp_vec) * scale
after, stat = fxp_ctor(8, 4, inp_vec)
after = after._data
print(stat)
status = fxp_computeStatus(before, after, 2**4)
print(before)
print(after)
print(status)
assert status.overflow==True, "Problem with fxp_computeStatus()"
assert status.overflow_count==4, "Problem with fxp_computeStatus()"



print('')
inp_vec = [ 16,  18, -4, -32, 32, -10, -64]
out_vec = [-16, -14, -4,   0,  0, -10,   0]

scale = 2**5
before = np.array(inp_vec) * scale
after, stat = fxp_ctor(10, 5, inp_vec)
after = after._data
print(stat)
status = fxp_computeStatus(before, after, 2**4)
print(before)
print(after)
print(status)
assert status.overflow==True, "Problem with fxp_computeStatus()"
assert status.overflow_count==5, "Problem with fxp_computeStatus()"



print('\n\nfxp from matrix tests:')
total_width = 10
frac_width = 5
mat_inp = [
    [1, 2, 3, 4],
    [2, 5, 7, 2],
    [9, 3, 5, 0],
]

fxp_mat, stat = fxp_ctor(total_width, frac_width, mat_inp)
print(stat)
fxp_printValue(fxp_mat)

