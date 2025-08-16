li t0, -1
li t1, 2
blt t0, t1, blt_label
li t2, 1
blt_label:
bltu t0, t1, bltu_label1
li t2, 2
bltu_label1:
bltu t1, t0 bltu_label2
li t2, 3
bltu_label2: