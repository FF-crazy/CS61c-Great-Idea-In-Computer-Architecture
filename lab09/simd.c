#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS; i++) {
			if(vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				   // This is where you should put your final result!
	/* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */
    for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
      __m128i temp = _mm_setzero_si128();
      for (unsigned int i = 0; i < NUM_ELEMS / 8 * 8; i += 8) {
        __m128i a =
            _mm_set_epi32(vals[i], vals[i + 1], vals[i + 2], vals[i + 3]);
        __m128i b =
            _mm_set_epi32(vals[i + 4], vals[i + 5], vals[i + 6], vals[i + 7]);
        a = _mm_and_si128(_mm_cmpgt_epi32(a, _127), a);
        b = _mm_and_si128(b, _mm_cmpgt_epi32(b, _127));
        a = _mm_add_epi32(a, b);
        temp = _mm_add_epi32(temp, a);
      }
      int p[4];
      _mm_storeu_si128((__m128i*)p, temp);
      result += p[0];
      result += p[1];
      result += p[2];
      result += p[3];
    }
    for (unsigned int i = NUM_ELEMS / 8 * 8; i < NUM_ELEMS; i++) {
      if (vals[i] >= 128) {
        result += vals[i];
      }
    }
    clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
  clock_t start = clock();
  __m128i _127 = _mm_set1_epi32(127);
  long long int result = 0;
  for (unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
    __m128i temp = _mm_setzero_si128();
    for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16) {
      __m128i a1 =
          _mm_set_epi32(vals[i], vals[i + 1], vals[i + 2], vals[i + 3]);
      __m128i b1 =
          _mm_set_epi32(vals[i + 4], vals[i + 5], vals[i + 6], vals[i + 7]);
      a1 = _mm_and_si128(_mm_cmpgt_epi32(a1, _127), a1);
      b1 = _mm_and_si128(_mm_cmpgt_epi32(b1, _127), b1);
      __m128i sum1 = _mm_add_epi32(a1, b1);

      __m128i a2 =
          _mm_set_epi32(vals[i + 8], vals[i + 9], vals[i + 10], vals[i + 11]);
      __m128i b2 =
          _mm_set_epi32(vals[i + 12], vals[i + 13], vals[i + 14], vals[i + 15]);
      a2 = _mm_and_si128(_mm_cmpgt_epi32(a2, _127), a2);
      b2 = _mm_and_si128(_mm_cmpgt_epi32(b2, _127), b2);
      __m128i sum2 = _mm_add_epi32(a2, b2);

      temp = _mm_add_epi32(temp, sum1);
      temp = _mm_add_epi32(temp, sum2);
    }

    int p[4];
    _mm_storeu_si128((__m128i*)p, temp);
    result += p[0] + p[1] + p[2] + p[3];
  }

  for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++) {
    if (vals[i] >= 128) {
      result += vals[i];
    }
  }

  clock_t end = clock();
  printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
  return result;
}