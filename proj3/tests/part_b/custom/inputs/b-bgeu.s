li t0, 2
li t1, -1
bge t0, t1, bge_label
li t2, 1
bge_label:
bgeu t0, t1, bgeu_label1
li t2, 2
bgeu_label1:
bgeu t1, t0, bgeu_label2
li t2, 3
bgeu_label2: