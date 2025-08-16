main:
    li s0, 0xf00 # the start of the array

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

    addi s1, s0, 36
    li t0, -3
    sw t0, 0(s1)
    li t0, 15
    sw t0, 4(s1)
    li t0, 24
    sw t0, 8(s1)
    li t0, 36
    sw t0, 12(s1)
    li t0, 13
    sw t0, 16(s1)
    li t0, 35
    sw t0, 20(s1)
    li t0, 142
    sw t0, 24(s1)
    li t0, 771
    sw t0, 28(s1)
    li t0, -103
    sw t0, 32(s1)

    mv a0, s0 
    mv a1, s1 
    li a2, 9
    li a3, 1
    li a4, 1
    call dot

    j finish


# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:

    # Prologue
    addi sp, sp, -12
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)

    # check arguments
    mv s0, a0  # s0 (int*) is the pointer to the start of v0
    mv a0, a2 
    call check_length

    mv s1, a1 # s1 (int*) is the pointer to the start of v1
    mv a0, a3
    mv a1, a4 
    call check_stride

    li a0, 0 # a0 is the result of dot multiplication
    li t0, 0 # t0 is the index of the loop
    li t2, 4
    mul t1, a3, t2 # t1 is the byte strides of v0
    mul t2, a4, t2 # t2 is the byte strides of v1

loop_start:
    bge t0, a2, loop_end
    lw t3, 0(s0)  # t3 is the value 
    lw t4, 0(s1)

    mul t3, t3, t4 
    add a0, a0, t3 # a0 += t3

    add s0, s0, t1  # s0 is the byte address of the elem of v0
    add s1, s1, t2 # s1 is the byte address of the elem of v1
    addi t0, t0, 1
    j loop_start

loop_end:


    # Epilogue
    lw s1, 8(sp)
    lw s0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 12
    
    ret


# =======================================================
# FUNCTION: Check if the length of vector is less 1
# Arguments:
#   a0 (int)  is the length of the vectors
# Returns:
#   null
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# =======================================================
check_length:
    li t0, 1
    bge a0, t0, skip_exit_1
    li a1, 75
    j finish
skip_exit_1:
    ret


# =======================================================
# FUNCTION: Check if the stride of either vector is less than 1
# Arguments:
#   a0 (int)  is the stride of the first vectors
#   a1 (int)  is the stride of the second vectors
# Returns:
#   null
# Exceptions:
#   If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
check_stride:
    li t0, 1
    bge a0, t0, skip_exit_2
    li a1, 76
    j finish
skip_exit_2:
    bge a1, t0, skip_exit_3
    li a1, 76
    j finish
skip_exit_3:
    ret

finish: