li x20, 0xffff
srli t0, x20, 0x5
srli t0, x20, 31 # 在I-Type中，移位的立即数只有5bit，最多位31

li x20, 0x1fff
srli t0, x20, 0x5
srli t0, x20, 31