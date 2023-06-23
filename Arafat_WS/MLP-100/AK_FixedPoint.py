from dataclasses import dataclass
import numpy as np


# Fixed-point datatype (struct)
@dataclass
class fxp_FixedPoint:
    _total_width: int
    _frac_width: int
    _scale_fact: int
    _data: np.ndarray
    _basetype: None    # will be initialize by constructor, should match _data.dtype


# This class can be used to retrieved the status of operation
@dataclass
class fxp_Status:
    overflow: bool
    overflow_count: int
    # maximum value in integer representation before fitting (sign-extension)
    max_vali: int 
    min_vali: int
    # maximum value in floating-point representation before fitting (sign-extension)
    max_valf: float
    min_valf: float




# This is needed because of Python's pass-by-ref mechanism. Should not be needed in C.
def fxp_copyStatus(dest, src):
    dest.overflow = src.overflow
    dest.overflow_count = src.overflow_count
    dest.max_vali = src.max_vali
    dest.min_vali = src.min_vali
    dest.max_valf = src.max_valf
    dest.min_valf = src.min_valf


# This function can be used to accumulates errors from several consecutive operations
# accumulates src onto des status object.
def fxp_accumulateStatus(dest, src):
    dest.overflow = (dest.overflow or src.overflow)    # any overflow means overall overflow
    dest.overflow_count += src.overflow_count
    dest.max_vali = max(src.max_vali, dest.max_vali)
    dest.min_vali = min(src.min_vali, dest.min_vali)
    dest.max_valf = max(src.max_valf, dest.max_valf)
    dest.min_valf = min(src.min_valf, dest.min_valf)

        
# fixed-point constructor: Returns a fixed-point object
def fxp_ctor(total_width, frac_width, array, dtype=None):
    assert frac_width <= total_width, "Invalid widths specified"
    # Compute the derived parameters
    scale = 2**frac_width    # scaling factor
    #if dtype==None:
    #    if total_width<=32: dtype = np.int32
    #    else: dtype = np.int64
    dtype = np.int64
    # convert the input array into fixed-point integer representation
    nparray = np.array(array) * scale   # scale the input numbers
    nparray = nparray.astype(dtype)     # convert to integer type
    fpitem = fxp_FixedPoint(total_width, frac_width, scale, nparray, dtype)
    fxp_fitData(fpitem, False)                 # fit within the specified width
    return fpitem


# checks if given fixed-point numbers are compatible
def fxp_isCompatible(a, b):
    assert a._total_width == b._total_width, "Total widths unequal"
    assert a._frac_width == b._frac_width, "Fraction widths unequal"
    assert a._scale_fact == b._scale_fact, "Scaling factor unequal"
    assert a._data.dtype == b._data.dtype, "Base types are different"
    return True    # reaching this line means they are compatible

    
# Returns a bit-mask where the lower _total_width bits are 1s, 
# all other bits are zeros.
def fxp_getMask(fxp_num):
    mask = (1 << fxp_num._total_width) - 1
    return mask


# Returns an array where values are 0 or 1.
# Elements with 1 in their _total_width (msb) bit position with 1 is 1 (negative number)
# Elements with 0 in their _total_width (msb) bit posistion with 0 is 0 (positive number)
def fxp_getSignBits(fxp_num):
    msb_pos = fxp_num._total_width - 1         # msb index
    sign_bits = fxp_num._data & (1<<msb_pos)   # zero/non-zero
    sign_bits = np.where(sign_bits!=0, 1, 0)   # convert non-zeros to 1
    return sign_bits


# Returns an array as floating point number
def fxp_getAsFloat(fxp_num):
    retval = fxp_num._data.astype(np.float32) / fxp_num._scale_fact
    return retval


# Returns the _data array as an array of binary-representation string
def fxp_getAsBin(fxp_num):
    retval = []
    for num in fxp_num._data: 
        retval.append(np.binary_repr(num))
    return retval


# Internal (private) function to compute the inaccuracies before and after fxp_fitData().
# before, after: numpy arrays (_data)
# Returns a fxp_Status object.
def fxp_computeStatus(before, after, scale):
    imin = np.min(before)
    imax = np.max(before)
    fmin = imin / scale
    fmax = imax / scale
    error_count = np.count_nonzero(before != after)
    if error_count>0: overflow = True
    else: overflow = False
    return fxp_Status(overflow, error_count, imax, imin, fmax, fmin)
    
    
# Truncate the underlying data to fit the fixed-point precision.
# This is done by duplicating the bit at MSb of the fixed-point for sign extension.
# If compute_status=True, it also computes the status.
# Returns a fxp_Status object.
def fxp_fitData(fxp_num, compute_status):
    mask = fxp_getMask(fxp_num)      # get the mask for bit selection
    cleared = fxp_num._data & mask   # clear upper bits
    imask = ~mask                    # inverted mask for sign extension of negative numbers
    sign_bits = fxp_getSignBits(fxp_num)    # make negative number filter
    new_data = cleared | (sign_bits * imask)  # set all upper bits to 1 of negative numbers (sign extension)
    if compute_status: retStat = fxp_computeStatus(fxp_num._data, new_data, fxp_num._scale_fact)
    else: retStat = None
    fxp_num._data = new_data    # update the fixed-point with the new data
    return retStat              # return None or computed status
    

# Returns a representation string
def fxp_repr(fxp_num):
    mstr = ['fxp_FixedPoint:']
    mstr.append(f'  total_width: {fxp_num._total_width}')
    mstr.append(f'  frac_width: {fxp_num._frac_width}')
    mstr.append(f'  scale_fact: {fxp_num._scale_fact}')
    mstr.append(f'  data: type{type(fxp_num._data)}  base-type: {fxp_num._data.dtype}  shape: {fxp_num._data.shape}')
    return '\n'.join(mstr)


# Returns an instance of fxp_FixedPoint with compatible parameters as template.
# _data remains invalid.
def fxp_makeSame(template):
    retobj = fxp_FixedPoint(
        template._total_width,
        template._frac_width,
        template._scale_fact,
        None,
        template._basetype, 
    )
    return retobj


# Returns an instance of fxp_FixedPoint with wider parameters as template.
# factor: is used to make the fxp fields wider
# _data remains invalid.
def fxp_makeWider(template, factor):
    assert factor > 0, "factor needs to be > 0"
    total_width = template._total_width * factor
    frac_width = template._frac_width * factor
    scale_fact = 2**frac_width    # scaling factor
    retobj = fxp_FixedPoint(
        total_width,
        frac_width,
        scale_fact,
        None,
        template._basetype, 
    )
    return retobj


# Returns a copy of the fixed-point number
def fxp_copy(fxp_num):
    retobj = fxp_makeSame(fxp_num)
    retobj._data = fxp_num._data.copy()
    return retobj
    



# ---- Printing utilities ----

# Prints a representation
def fxp_printInfo(fxp_num):
    print(fxp_repr(fxp_num))


# Prints the internal array (_data)
def fxp_printData(fxp_num):
    print(fxp_num._data)


# Prints the fixed point numbers as floating points
def fxp_printValue(fxp_num):
    nparr = fxp_getAsFloat(fxp_num)
    print(nparr)


# Prints the fixed point numbers as binary strings
def fxp_printBinstr(fxp_num):
    bin_str = fxp_getAsBin(fxp_num)
    print(bin_str)




# ---- Math Operations ----

# Adds two Fixed point numbers
# status_obj: instance of fxp_Status to get status back
def fxp_add(a, b, status_obj=None):
    fxp_isCompatible(a, b)
    sum_data = a._data + b._data
    retobj = fxp_makeSame(a)
    retobj._data = sum_data
    fit_stat = fxp_fitData(retobj, True)
    if status_obj: fxp_copyStatus(status_obj, fit_stat)
    return retobj


# Subtracts two Fixed point numbers
# status_obj: instance of fxp_Status to get status back
def fxp_sub(a, b, status_obj=None):
    fxp_isCompatible(a, b)
    sub_data = a._data - b._data
    retobj = fxp_makeSame(a)
    retobj._data = sub_data
    fit_stat = fxp_fitData(retobj, True)
    if status_obj: fxp_copyStatus(status_obj, fit_stat)
    return retobj
    

# Multiplier two Fixed point numbers
# status_obj: instance of fxp_Status to get status back
def fxp_mult(a, b, status_obj=None):
    fxp_isCompatible(a, b)
    frac_width = a._frac_width
    mult_data = (a._data * b._data) >> frac_width   # multiply scale-down
    retobj = fxp_makeSame(a)
    retobj._data = mult_data
    fit_stat = fxp_fitData(retobj, True)
    if status_obj: fxp_copyStatus(status_obj, fit_stat)
    return retobj



