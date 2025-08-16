.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 72.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 73.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 74.
# =======================================================
matmul:

    # Error checks
    li t0, 1

    bge a1, t0, skip_matmul_exit_1
    li a1, 72
    call exit2
skip_matmul_exit_1:
    bge a2, t0, skip_matmul_exit_2
    li a1, 72
    call exit2
skip_matmul_exit_2:
    bge a4, t0, skip_matmul_exit_3 
    li a1, 73
    call exit2
skip_matmul_exit_3:
    bge a5, t0, skip_matmul_exit_4
    li a1, 73 
    call exit2 
skip_matmul_exit_4:
    beq a2, a4, skip_matmul_exit_5
    li a1, 74
    call exit2

skip_matmul_exit_5:

    # Prologue
    addi sp, sp, -44
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)
    sw s7, 32(sp)
    sw s8, 36(sp)
    sw s9, 40(sp)


    # move the arguments to the saved registers
    mv s0, a0 # s0 is the pointer to the start of m0 
    mv s1, a1 # s1 is the # of rows (height) of m0 & d
    mv s2, a2 # s2 is the # of columns (width) of m0
              # s2 is the # of rows (height) of m1
    mv s3, a3 # s3 is the pointer to the start of m1
    mv s4, a5 # s4 is the # of columns (width) of m1 & d
    mv s5, a6 # s5 is the pointer to the the start of d

    # initialize the variables
    li t1, 4
    mul s6, t1, s2 # s6 is the # byte of row of m0

    li s7, 0 # s7 is i which is the row of current element of d

outer_loop_start:
    bge s7, s1, outer_loop_end
    li s8, 0 # s8 is j which is the col of current element of d
    mv s9, s3 # s9 is the column pointer of m1

inner_loop_start:
    bge s8, s4, inner_loop_end
    
    # set the argument of dot product
    # a0 is the pointer to the start of v0
    mv a0, s0 
    # a1 is the pointer to the start of v1
    mv a1, s9 
    # a2 is the length of the vectors, i.e. the column of m0
    mv a2, s2
    # a3 is the stride of v0 (unit 1)
    li a3, 1
    # a4 is the stride of v1 (unit 2)
    mv a4, s4

    call dot

    ebreak
    # a0 is the result of d[i][j]
    sw a0, 0(s5)

    addi s8, s8, 1
    addi s9, s9, 4
    addi s5, s5, 4
    j inner_loop_start
inner_loop_end:
    addi s7, s7, 1
    add s0, s0, s6 
    j outer_loop_start
outer_loop_end:


    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    lw s8, 36(sp)
    lw s9, 40(sp)
    addi sp, sp, 44
    
    ret