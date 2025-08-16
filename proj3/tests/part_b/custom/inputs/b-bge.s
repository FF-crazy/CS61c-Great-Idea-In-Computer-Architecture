li t0, 2
li t1, 1
bge t0, t1, bge_success
li t2, 1
bge_success:
li s0, -1
li s1, 2
bge s0, s1, bge_fail1
li t2, 2
bge_fail1:
li s0, 1
li s1, 1
bge s0, s1, bge_fail2
li t2, 3
bge_fail2: