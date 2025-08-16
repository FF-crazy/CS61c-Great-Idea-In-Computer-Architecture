li t0, 1
li t1, 2
blt t0, t1, blt_success
li t2, 2
blt_success:
li s0, 2
li s1, -1
blt s0, s1, blt_fail1
li t2, 2
blt_fail1:
li s0, 1
li s1, 1
blt s0, s1, blt_fail2
li t2, 3
blt_fail2: