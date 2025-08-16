main:
    li a0, 0
    call abs
    li a0, -1
    call abs 
    li a0, 1
    call abs
    j finish
# =================================================================
# FUNCTION: Given an int return its absolute value.
# Arguments:
# 	a0 (int) is input integer
# Returns:
#	a0 (int) the absolute value of the input
# =================================================================
abs:
    # Prologue
    # return 0
    # mv a0, zero

    # branch if positive
    bge a0, zero, done
    # invert a if negative
    sub a0, zero, a0
done:
    # Epilogue

    ret

finish: