#define ec_fopen 93
#define ec_fwrite 94
#define ec_fclose 95
#define ec_malloc 88

.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 93.
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 94.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 95.
# ==============================================================================
write_matrix:

    # Prologue
    addi sp, sp, -24
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)

    mv s0, a0 # s0 is the pointer to string representing the filename
    mv s1, a1 # s1 is the pointer to the start of the matrix in memory
    mv s2, a2 # s2 is the number of rows in the matrix
    mv s3, a3 # s3 is the number of columns in the matrix

    # open the file
    mv a1, a0
    li a2, 1 # write mode 
    call fopen 
    li t0, -1
    bne a0, t0, skip_write_matrix_fopen_exit
    mv a1, s0 
    call print_str
    li a1, ec_fopen
    call exit2
skip_write_matrix_fopen_exit:

    mv s0, a0 # s0 is the file description
    # allocate the memory to store the row and col
    li a0, 8
    call malloc
    bne a0, zero, skip_write_matrix_malloc_exit
    li a1, ec_malloc
    call exit2
skip_write_matrix_malloc_exit:

    mv s4, a0 # s4 now is the memory to store the row and col

    # store the row & col to the memory
    sw s2, 0(s4)
    sw s3, 4(s4)

    # write the row & col to the memory
    mv a1, s0 
    mv a2, s4 
    li a3, 2
    li a4, 4
    call fwrite
    li t0, 2 
    bge a0, t0, skip_write_matrix_1st_fwrite_exit
    # flush
    mv a1, s0 
    call fflush 
    li a1, ec_fwrite
    call exit2
skip_write_matrix_1st_fwrite_exit:
    # free the memroy 
    mv a0, s4 
    call free

    # store the matrix
    mul s2, s2, s3 # s2 = row * col
    mv a1, s0
    mv a2, s1
    mv a3, s2 
    li a4, 4
    call fwrite
    bge a0, s2, skip_write_matrix_2nd_fwrite_exit
    # flush
    mv a1, s0 
    call fflush 
    li a1, ec_fwrite
    call exit2
skip_write_matrix_2nd_fwrite_exit:

    # flush
    mv a1, s0
    call fflush

    # close the file 
    mv a1, s0 
    call fclose
    beq a0, zero, skip_write_matrix_fwrite_exit 
    li a1, ec_fclose
    call exit2
skip_write_matrix_fwrite_exit:

    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    addi sp, sp, 24
    ret
