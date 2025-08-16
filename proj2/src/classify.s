#define ec_arg_num 89
#define ec_malloc 88

.globl classify

.data 
print_m0_str: .string "m0: \n"

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    # prelogue
    addi sp, sp, -40
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

    mv s0, a1 # s0 is the argv
    mv s1, a2 # s1 is the print_classification flag
    
    # =====================================
    # CHECK THE NUMBER OF ARGUMENT
    # =====================================

    li t0, 5 # the first argument is main.s
    beq a0, t0, skip_check_arg_num_exit

    li a1, ec_arg_num
    call exit2
skip_check_arg_num_exit:

	# =====================================
    # LOAD MATRICES
    # =====================================

    # allocate 24 bytes for row&col of 3 matrix
    li a0, 24
    call malloc 
    bne a0, zero, skip_classify_1st_malloc_exit
    li a1, ec_malloc
    call exit2
skip_classify_1st_malloc_exit:
    mv s2, a0  # s2 is the address of row & col of three matrices
               # s2[0, 4, 8, 12, 16, 20] = [r0, c0, r1, c1, ri, ci]

    # Load pretrained m0
    lw a0, 4(s0)
    addi a1, s2, 0
    addi a2, s2, 4
    call read_matrix
    mv s3, a0  # s3 is the m0 matrix

    # print m0 matrix
    # mv a0, s3 
    # lw a1, 0(s2)
    # lw a2, 4(s2)
    # call print_int_array
    # li t0, '\n'
    # call print_char

    # Load pretrained m1
    lw a0, 8(s0)
    addi a1, s2, 8
    addi a2, s2, 12
    call read_matrix
    mv s4, a0 # s4 is the m1 matrix

    # print m1 matrix
    # mv a0, s4
    # lw a1, 8(s2)
    # lw a2, 12(s2)
    # call print_int_array
    # li t0, '\n'
    # call print_char

    # Load input matrix
    lw a0, 12(s0)
    addi a1, s2, 16
    addi a2, s2, 20
    call read_matrix
    mv s5, a0 # s5 is the input matrix

    # print input matrix
    # mv a0, s5
    # lw a1, 16(s2)
    # lw a2, 20(s2)
    # call print_int_array
    # li t0, '\n'
    # call print_char

    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)


    # 1. LINEAR LAYER:    m0 * input
    # allocate the memory of d
    lw t0, 0(s2) # t0 is the row of m0 matrix
    lw t1, 20(s2) # t1 is the col of input matrix
    mul s6, t0, t1 # t0 is the # elem of product
    li t0, 4
    mul a0, t0, s6 # t0 is the # bytes of product
    call malloc
    bne a0, zero, skip_classify_2nd_malloc_exit
    li a1, ec_malloc
    call exit2
skip_classify_2nd_malloc_exit:

    # 1. LINEAR LAYER:    m0 * input
    # cal matmul
    mv a6, a0  # a6 = a0 is the address of product
    mv a0, s3
    lw a1, 0(s2)
    lw a2, 4(s2)
    mv a3, s5
    lw a4, 16(s2)
    lw a5, 20(s2)
    call matmul 
    mv s7, a6 # a6 is the result of first product

    # print m0 *  input
    # mv a0, s7
    # lw a1, 0(s2)
    # lw a2, 20(s2)
    # call print_int_array
    # li t0, '\n'
    # call print_char

    # free the m0 and input
    mv a0, s3
    call free 
    mv a0, s5
    call free

    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    mv a0, s7 
    mv a1, s6
    call relu

    # print ReLU(m0 * input)
    # mv a0, s7
    # lw a1, 0(s2)
    # lw a2, 20(s2)
    # call print_int_array
    # li t0, '\n'
    # call print_char

    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    # allocate the memory of the product
    lw t0, 8(s2)
    lw t1, 20(s2)
    mul s8, t0, t1
    li t0, 4
    mul a0, s8, t0
    call malloc
    bne a0, zero, skip_classify_3rd_malloc_exit
    li a1, ec_malloc
    call exit2
skip_classify_3rd_malloc_exit:
    mv a6, a0
    mv a0, s4
    lw a1, 8(s2)
    lw a2, 12(s2)
    mv a3, s7 
    lw a4, 0(s2)
    lw a5, 20(s2)
    call matmul 
    mv s3, a6 # s3 now is the pointer of m1 * ReLU(m0 * input)

    # print m1 * ReLU(m0 * input)
    # mv a0, s3
    # lw a1, 8(s2)
    # lw a2, 20(s2)
    # call print_int_array
    # li t0, '\n'
    # call print_char

    # free the m1
    mv a0, s4 
    call free 
    # free the m0 * input
    mv a0, s7 
    call free

    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix

    lw a0, 16(s0)
    mv a1, s3
    lw a2, 8(s2)
    lw a3, 20(s2)
    call write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax
    mv a0, s3 
    mv a1, s8
    call argmax
    mv s4, a0  # s0 is the classification result

    # free the column and row array
    mv a0, s2 
    call free 
    # free the m1 * ReLU(m0 * input)
    mv a0, s3 
    call free
    # Print classification
    bne s1, zero, skip_print_classification
    mv a1, s4 
    call print_int
    
skip_print_classification:

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
    addi sp, sp, 40
    ret
