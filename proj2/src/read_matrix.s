#define ec_malloc 88 # ec means error exit code
#define ec_fopen 90
#define ec_fread 91
#define ec_fclose 92

.globl read_matrix

.data 
new_line: .asciiz "\r\n"

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -20
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)

	mv s0, a0 # s0 is the pointer to string representing the filename
    mv s1, a1 # s1 is a pointer to an integer, we will set it to the number of rows
    mv s2, a2 # s2 is a pointer to an integer, we will set it to the number of columns

    # call fopen() open the file
    mv a1, s0 
    li a2, 0
    call fopen  # a0 is the file descriptor
    li t0 -1
    bne a0, t0, skip_fopen_exit
    li a1, ec_fopen
    call exit2
skip_fopen_exit:

    mv s0, a0  # now s0 is the file descrptor
    # allocate a word to store the column and row
    li a0, 8
    call malloc
    bne a0, zero, skip_malloc_exit_1
    li a1, ec_malloc
    call exit2
skip_malloc_exit_1:

    # read the first word
    mv a1, s0 
    mv a2, a0
    li a3, 8
    call fread
    li t0, 8
    beq a3, t0, skip_read_first_8_bytes_exit
    li a1, ec_fread
    call exit2
skip_read_first_8_bytes_exit:
    # parse the first 8 bytes to the column and row, a2 is the read number
    lw t0, 0(a2) # t0 is row
    sw t0, 0(s1)
    lw t1, 4(a2) # t1 is col
    sw t1, 0(s2)

    mul s3, t0, t1  # s3 is the number of the elements of matrix
    li t0, 4
    mul s3, s3, t0 # now s3 is the number of the bytes of matrix

    # allocate the memory of the matrix
    mv a0, s3  # t0 is the size of the matrix
    call malloc  # a0 is the memory pointer
    bne a0, zero, skip_malloc_exit_2
    li a1, ec_malloc
    call exit2
skip_malloc_exit_2:

    # read the matrix
    mv a1, s0 
    mv a2, a0
    mv a3, s3 
    call fread 

    
    # check the fread
    beq a0, s3, skip_fread_matrix_exit
    li a1, ec_fread
    call exit2
skip_fread_matrix_exit:
    mv s3, a2 

    # close the file
    mv a1, s0
    call fclose
    beq a0, zero, skip_fclose_exit 
    li a1, ec_fclose
    call exit2
skip_fclose_exit:
    
    mv a0, s3

    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    addi sp, sp, 20

    ret
