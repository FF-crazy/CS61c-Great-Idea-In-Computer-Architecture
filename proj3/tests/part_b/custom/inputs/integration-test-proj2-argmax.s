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
    call argmax

    j finish

# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================

argmax:
    # Prologue
    # check argument
    li t0, 1
    bge a1, t0, skip_exit
    li a1, 77
    j finish
skip_exit: 
    li t0, 0 # t0 is the max index
    li t1, 4 # t1 is the stride
    li t2, 1 # t2 is the index
    lw t3, 0(a0) # t3 is the max element

loop_start:
    bge t2, a1, loop_end
    mul t4, t2, t1 # t4 is the offset
    add t5, a0, t4 # a5 is the address of current element
    lw t6, 0(t5) # t5 is the current element
    ble t6, t3, loop_continue
    mv t0, t2 
    mv t3, t6

loop_continue:
    addi t2, t2, 1
    j loop_start


loop_end:
    
    mv a0, t0
    # Epilogue
    ret


finish: