/* The MIT License

   Copyright (c) 2008, 2011 Attractive Chaos <attractor@live.co.uk>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/*
  2011-04-10 (0.1.6):

  	* Added sample

  2011-03 (0.1.5):

	* Added shuffle/permutation

  2008-11-16 (0.1.4):

    * Fixed a bug in introsort() that happens in rare cases.

  2008-11-05 (0.1.3):

    * Fixed a bug in introsort() for complex comparisons.

	* Fixed a bug in mergesort(). The previous version is not stable.

  2008-09-15 (0.1.2):

	* Accelerated introsort. On my Mac (not on another Linux machine),
	  my implementation is as fast as std::sort on random input.

	* Added combsort and in introsort, switch to combsort if the
	  recursion is too deep.

  2008-09-13 (0.1.1):

	* Added k-small algorithm

  2008-09-05 (0.1.0):

	* Initial version

*/

#ifndef RADIX_SORT_H
#define RADIX_SORT_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define RADIS_SORT_MIN_SIZE 64
#define RADIS_SORT_MAX_BITS 8

#endif

#ifndef RADIX_SORT_TYPE
#error "Must define RADIX_SORT_TYPE"
#endif

#ifndef RADIX_SORT_NAME
#define RADIX_SORT_NAME_DEFINED
#define RADIX_SORT_NAME RADIX_SORT_TYPE
#endif

#ifndef RADIX_SORT_KEY
#define RADIX_SORT_KEY(x) (x)
#endif

#ifndef RADIX_SORT_KEY_SIZE
#define RADIX_SORT_KEY_SIZE sizeof(RADIX_SORT_TYPE)
#endif

#define RADIX_SORT_CONCAT_(a, b) a ## b
#define RADIX_SORT_CONCAT(a, b) RADIX_SORT_CONCAT_(a, b)
#define RADIX_SORT_FUNC(name) RADIX_SORT_CONCAT(name##_, RADIX_SORT_NAME)

#define RADIX_SORT_BUCKET_TYPE RADIX_SORT_CONCAT(RADIX_SORT_NAME, _bucket_t)

typedef struct {
	RADIX_SORT_TYPE *b, *e;
} RADIX_SORT_BUCKET_TYPE;

static inline void RADIX_SORT_FUNC(rs_insertsort)(RADIX_SORT_TYPE *beg, RADIX_SORT_TYPE *end)
{
	RADIX_SORT_TYPE *i;
	for (i = beg + 1; i < end; ++i) {
		if (RADIX_SORT_KEY(*i) < RADIX_SORT_KEY(*(i - 1))) {
			RADIX_SORT_TYPE *j, tmp = *i;
			for (j = i; j > beg && RADIX_SORT_KEY(tmp) < RADIX_SORT_KEY(*(j-1)); --j)
				*j = *(j - 1);
			*j = tmp;
		}
	}
}

static inline void RADIX_SORT_FUNC(rs_sort)(RADIX_SORT_TYPE *beg, RADIX_SORT_TYPE *end, int n_bits, int s)
{
	RADIX_SORT_TYPE *i;
	int size = 1<<n_bits, m = size - 1;
	RADIX_SORT_BUCKET_TYPE *k, b[1<<RADIS_SORT_MAX_BITS], *be = b + size;
	assert(n_bits <= RADIS_SORT_MAX_BITS);
	for (k = b; k != be; ++k) k->b = k->e = beg;
	for (i = beg; i != end; ++i) ++b[RADIX_SORT_KEY(*i)>>s&m].e;
	for (k = b + 1; k != be; ++k) {
		k->e += (k-1)->e - beg, k->b = (k-1)->e;
	}
	for (k = b; k != be;) {
		if (k->b != k->e) {
			RADIX_SORT_BUCKET_TYPE *l;
			if ((l = b + (RADIX_SORT_KEY(*k->b)>>s&m)) != k) {
				RADIX_SORT_TYPE tmp = *k->b, swap;
				do {
					swap = tmp; tmp = *l->b; *l->b++ = swap;
					l = b + (RADIX_SORT_KEY(tmp)>>s&m);
				} while (l != k);
				*k->b++ = tmp;
			} else ++k->b;
		} else ++k;
	}
	for (b->b = beg, k = b + 1; k != be; ++k) k->b = (k-1)->e;
	if (s) {
		s = s > n_bits? s - n_bits : 0;
		for (k = b; k != be; ++k)
			if (k->e - k->b > RADIS_SORT_MIN_SIZE) RADIX_SORT_FUNC(rs_sort)(k->b, k->e, n_bits, s);
			else if (k->e - k->b > 1) RADIX_SORT_FUNC(rs_insertsort)(k->b, k->e);
	}
}

static inline void RADIX_SORT_FUNC(radix_sort)(RADIX_SORT_TYPE *beg, RADIX_SORT_TYPE *end)
{
	if (end - beg <= RADIS_SORT_MIN_SIZE) RADIX_SORT_FUNC(rs_insertsort)(beg, end);
	else RADIX_SORT_FUNC(rs_sort)(beg, end, RADIS_SORT_MAX_BITS, (RADIX_SORT_KEY_SIZE - 1) * RADIS_SORT_MAX_BITS);
}

#undef RADIX_SORT_CONCAT_
#undef RADIX_SORT_CONCAT
#undef RADIX_SORT_FUNC

#ifdef RADIX_SORT_NAME_DEFINED
#undef RADIX_SORT_NAME
#undef RADIX_SORT_NAME_DEFINED
#endif

#ifdef RADIX_SORT_LT_DEFINED
#undef RADIX_SORT_LT
#endif
#undef RADIX_SORT_LT_DEFINED
