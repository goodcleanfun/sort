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

#ifndef MERGESORT_H
#define MERGESORT_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#endif

#ifndef MERGESORT_TYPE
#error "Must define MERGESORT_TYPE"
#endif

#ifndef MERGESORT_NAME
#define MERGESORT_NAME_DEFINED
#define MERGESORT_NAME MERGESORT_TYPE
#endif

#ifndef MERGESORT_LT
#if !defined(MERGESORT_STR) && !defined(MERGESORT_AUX_TYPE)
#define MERGESORT_LT(a, b) ((a) < (b))
#elif !defined(MERGESORT_STR) && defined(MERGESORT_AUX_TYPE)
#define MERGESORT_LT(a, b, aux) (((aux)[(a)] < (aux)[(b)]))
#elif defined(MERGESORT_STR) && !defined(MERGESORT_AUX_TYPE)
#define MERGESORT_LT(a, b) (strcmp((a), (b)) < 0)
#else
#define MERGESORT_LT(a, b, aux) (strcmp((aux)[(a)], (aux)[(b)]) < 0)
#endif
#define MERGESORT_LT_DEFINED
#endif

#define MERGESORT_CONCAT_(a, b) a ## b
#define MERGESORT_CONCAT(a, b) MERGESORT_CONCAT_(a, b)
#define MERGESORT_FUNC(name) MERGESORT_CONCAT(MERGESORT_NAME, _##name)

static inline void MERGESORT_FUNC(mergesort_direction)(size_t n, MERGESORT_TYPE array[], MERGESORT_TYPE temp[], bool reverse)
{
	MERGESORT_TYPE *a2[2], *a, *b;
	int curr, shift;

	a2[0] = array;
	a2[1] = temp? temp : (MERGESORT_TYPE*)malloc(sizeof(MERGESORT_TYPE) * n);
	for (curr = 0, shift = 0; (1ul<<shift) < n; ++shift) {
		a = a2[curr]; b = a2[1-curr];
		if (shift == 0) {
			MERGESORT_TYPE *p = b, *i, *eb = a + n;
			for (i = a; i < eb; i += 2) {
				if (i == eb - 1) *p++ = *i;
				else {
					#ifndef MERGESORT_AUX_TYPE
					if (MERGESORT_LT(*(i+1), *i) ^ reverse)
					#else
					if (MERGESORT_LT(*(i+1), *i, aux) ^ reverse)
					#endif
					{
						*p++ = *(i+1); *p++ = *i;
					} else {
						*p++ = *i; *p++ = *(i+1);
					}
				}
			}
		} else {
			size_t i, step = 1ul<<shift;
			for (i = 0; i < n; i += step<<1) {
				MERGESORT_TYPE *p, *j, *k, *ea, *eb;
				if (n < i + step) {
					ea = a + n; eb = a;
				} else {
					ea = a + i + step;
					eb = a + (n < i + (step<<1)? n : i + (step<<1));
				}
				j = a + i; k = a + i + step; p = b + i;
				while (j < ea && k < eb) {
					#ifndef MERGESORT_AUX_TYPE
					if (MERGESORT_LT(*k, *j) ^ reverse)
					#else
					if (MERGESORT_LT(*k, *j, aux) ^ reverse)
					#endif
					{
						*p++ = *k++;
					} else {
						*p++ = *j++;
					}
				}
				while (j < ea) *p++ = *j++;
				while (k < eb) *p++ = *k++;
			}
		}
		curr = 1 - curr;
	}
	if (curr == 1) {
		MERGESORT_TYPE *p = a2[0], *i = a2[1], *eb = array + n;
		for (; p < eb; ++i) *p++ = *i;
	}
	if (temp == 0) free(a2[1]);
}

// Original function (ascending order)
static inline void MERGESORT_FUNC(mergesort)(size_t n, MERGESORT_TYPE array[], MERGESORT_TYPE temp[])
{
	MERGESORT_FUNC(mergesort_direction)(n, array, temp, false);
}

// Reverse version (descending order)
static inline void MERGESORT_FUNC(mergesort_reverse)(size_t n, MERGESORT_TYPE array[], MERGESORT_TYPE temp[])
{
	MERGESORT_FUNC(mergesort_direction)(n, array, temp, true);
}

#undef MERGESORT_CONCAT_
#undef MERGESORT_CONCAT
#undef MERGESORT_FUNC

#ifdef MERGESORT_NAME_DEFINED
#undef MERGESORT_NAME
#undef MERGESORT_NAME_DEFINED
#endif

#ifdef MERGESORT_LT_DEFINED
#undef MERGESORT_LT
#endif
#undef MERGESORT_LT_DEFINED
