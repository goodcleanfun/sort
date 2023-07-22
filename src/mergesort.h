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
#include "common.h"

#define MERGESORT_INIT(name, type_t, __sort_lt)								        \
	static inline void ks_mergesort_##name(size_t n, type_t array[], type_t temp[])	\
	{																	            \
		type_t *a2[2], *a, *b;											            \
		int curr, shift;												            \
																		            \
		a2[0] = array;													            \
		a2[1] = temp? temp : (type_t*)malloc(sizeof(type_t) * n);		            \
		for (curr = 0, shift = 0; (1ul<<shift) < n; ++shift) {			            \
			a = a2[curr]; b = a2[1-curr];								            \
			if (shift == 0) {											            \
				type_t *p = b, *i, *eb = a + n;							            \
				for (i = a; i < eb; i += 2) {							            \
					if (i == eb - 1) *p++ = *i;							            \
					else {												            \
						if (__sort_lt(*(i+1), *i)) {					            \
							*p++ = *(i+1); *p++ = *i;					            \
						} else {										            \
							*p++ = *i; *p++ = *(i+1);					            \
						}												            \
					}													            \
				}														            \
			} else {													            \
				size_t i, step = 1ul<<shift;							            \
				for (i = 0; i < n; i += step<<1) {						            \
					type_t *p, *j, *k, *ea, *eb;						            \
					if (n < i + step) {									            \
						ea = a + n; eb = a;								            \
					} else {											            \
						ea = a + i + step;								            \
						eb = a + (n < i + (step<<1)? n : i + (step<<1));            \
					}													            \
					j = a + i; k = a + i + step; p = b + i;				            \
					while (j < ea && k < eb) {							            \
						if (__sort_lt(*k, *j)) *p++ = *k++;				            \
						else *p++ = *j++;								            \
					}													            \
					while (j < ea) *p++ = *j++;							            \
					while (k < eb) *p++ = *k++;							            \
				}														            \
			}															            \
			curr = 1 - curr;											            \
		}																            \
		if (curr == 1) {												            \
			type_t *p = a2[0], *i = a2[1], *eb = array + n;				            \
			for (; p < eb; ++i) *p++ = *i;								            \
		}																            \
		if (temp == 0) free(a2[1]);										            \
	}

#define ks_mergesort(name, n, a, t) ks_mergesort_##name(n, a, t)

#define MERGESORT_INIT_GENERIC(type_t) MERGESORT_INIT(type_t, type_t, ks_lt_generic)
#define MERGESORT_INIT_STR MERGESORT_INIT(str, ksstr_t, ks_lt_str)

#endif
