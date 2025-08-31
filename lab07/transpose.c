#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

static inline int min_int(int a, int b) {
  return a < b ? a : b;
}
/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int* dst, int* src) {
  for (int ii = 0; ii < n; ii += blocksize) {
    int i_end = min_int(ii + blocksize, n);
    for (int jj = 0; jj < n; jj += blocksize) {
      int j_end = min_int(jj + blocksize, n);
      for (int i = ii; i < i_end; ++i) {
        for (int j = jj; j < j_end; ++j) {
          dst[j + i * n] = src[i + j * n];
          // dst[j + i * n] = src[i + j * n];
        }
      }
    }
  }
}
