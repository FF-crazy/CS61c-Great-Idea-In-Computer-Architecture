li t0, 1
li t1, 1
beq t0, t1, beq_success
li t2, 2
beq_success:
li s0, 1
li s1, 2
beq s0, s1, beq_fail
li t2, 2
beq_fail: