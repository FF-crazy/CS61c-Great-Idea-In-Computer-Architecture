li x30, 100  # the start of the array
li x31, 0xfffffff0

sb, x31, 0(x30)
sh, x31, 4(x30)
sw, x31, 8(x30)

lb t0, 0(x30)  # sign-extension
lh t0, 0(x30)
lw t0, 0(x30)

lb t1, 4(x30)
lh t1, 4(x30)
lw t1, 4(x30)

lb t2, 8(x30)
lh t2, 8(x30)
lw t2, 8(x30)