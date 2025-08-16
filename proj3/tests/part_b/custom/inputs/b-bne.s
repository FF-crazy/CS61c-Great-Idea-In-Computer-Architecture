li t0, 1
li t1, 2
bne t0, t1, bne_success
li t2, 2
bne_success:
li s0, 1
li s1, 1
bne s0, s1, bne_fail
li t2, 2
bne_fail: