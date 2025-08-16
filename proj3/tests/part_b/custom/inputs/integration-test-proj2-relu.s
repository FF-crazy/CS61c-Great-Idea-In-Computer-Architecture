main:
    li s0, 0xffff # the start of the array

    li t0, -9
    sw t0, 0(s0)
    li t0, 5
    sw t0, 4(s0)
    li t0, 9
    sw t0, 8(s0)
    li t0, -3
    sw t0, 12(s0)
    li t0, 24
    sw t0, 16(s0)
    li t0, 36
    sw t0, 20(s0)
    li t0, 102
    sw t0, 24(s0)
    li t0, 77
    sw t0, 28(s0)
    li t0, -123
    sw t0, 32(s0)

    mv a0, s0 
    li a1, 9
    call relu

    j finish

# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    # Prologue

    li t0, 1
    bge a1, t0, skip_exit
    li a1, 78
    j finish

skip_exit:

    li t0, 4  # t0 is stride
    li t1, 0  # t1 is cnt
loop_start:
    beq t1, a1, loop_end


loop_continue:
    
    mul t2, t0, t1 # t2 is the offset
    add t3, a0, t2 # t3 is the pointer of current element
    lw t4, 0(t3) # t4 is the current element
    bge t4, zero, skip_relu  # if t4 >= 0, skip the  relu
    sw zero, 0(t3)

skip_relu:
    addi t1, t1, 1
    j loop_start

loop_end:
    # Epilogue
	ret

finish: