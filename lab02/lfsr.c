#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"
unsigned get_bit(unsigned x, unsigned n) {
  // YOUR CODE HERE
  // Returning -1 is a placeholder (it makes
  // no sense, because get_bit only returns
  // 0 or 1)
  x >>= n;
  return x & 1;
}
// Set the nth bit of the value of x to v.
// Assume 0 <= n <= 31, and v is 0 or 1
void set_bit(unsigned* x, unsigned n, unsigned v) {
  // YOUR CODE HERE
  unsigned temp = 1;
  temp <<= n;
  if (v) {
    *x |= temp;
  } else {
    *x &= ~temp;
  }
}
// Flip the nth bit of the value of x.
// Assume 0 <= n <= 31
void flip_bit(unsigned* x, unsigned n) {
  // YOUR CODE HERE
  unsigned temp = 1;
  temp <<= n;
  *x ^= temp;
}

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    unsigned val = get_bit(*reg, 0) ^ get_bit(*reg, 2) ^ get_bit(*reg, 3) ^
                   get_bit(*reg, 5);
    *reg >>= 1;
    set_bit((unsigned *)reg, 15, val);
}
