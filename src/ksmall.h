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

#ifndef KSMALL_H
#define KSMALL_H

#include <stdlib.h>
#include <string.h>
#include "common.h"

#define KSORT_SWAP(type_t, a, b) { register type_t t=(a); (a)=(b); (b)=t; }

#define KSMALL_INIT(name, type_t, __sort_lt)								    \
	/* This function is adapted from: http://ndevilla.free.fr/median/ */        \
	/* 0 <= kk < n */													        \
	static inline type_t ks_ksmall_##name(size_t n, type_t arr[], size_t kk)    \
	{																	        \
		type_t *low, *high, *k, *ll, *hh, *mid;							        \
		low = arr; high = arr + n - 1; k = arr + kk;					        \
		for (;;) {														        \
			if (high <= low) return *k;									        \
			if (high == low + 1) {										        \
				if (__sort_lt(*high, *low)) KSORT_SWAP(type_t, *low, *high);    \
				return *k;												        \
			}															        \
			mid = low + (high - low) / 2;								        \
			if (__sort_lt(*high, *mid)) KSORT_SWAP(type_t, *mid, *high);        \
			if (__sort_lt(*high, *low)) KSORT_SWAP(type_t, *low, *high);        \
			if (__sort_lt(*low, *mid)) KSORT_SWAP(type_t, *mid, *low);	        \
			KSORT_SWAP(type_t, *mid, *(low+1));							        \
			ll = low + 1; hh = high;									        \
			for (;;) {													        \
				do ++ll; while (__sort_lt(*ll, *low));					        \
				do --hh; while (__sort_lt(*low, *hh));					        \
				if (hh < ll) break;										        \
				KSORT_SWAP(type_t, *ll, *hh);							        \
			}															        \
			KSORT_SWAP(type_t, *low, *hh);								        \
			if (hh <= k) low = ll;										        \
			if (hh >= k) high = hh - 1;									        \
		}																        \
	}

#define ks_ksmall(name, n, a, k) ks_ksmall_##name(n, a, k)

#define KSMALL_INIT_GENERIC(type_t) KSMALL_INIT(type_t, type_t, ks_lt_generic)
#define KSMALL_INIT_STR KSMALL_INIT(str, ksstr_t, ks_lt_str)

#endif
