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

#endif

#ifndef KSMALL_TYPE
#error "KSMALL_TYPE must be defined"
#endif

#ifndef KSMALL_NAME
#define KSMALL_NAME KSMALL_TYPE
#endif

#ifndef KSMALL_SWAP
#define KSMALL_SWAP(KSMALL_TYPE, a, b) { register KSMALL_TYPE t=(a); (a)=(b); (b)=t; }
#define KSMALL_SWAP_DEFINED
#endif

#ifndef KSMALL_LT
#if !defined(KSMALL_STR)
#define KSMALL_LT(a, b) ((a) < (b))
#else
#define KSMALL_LT(a, b) (strcmp((a), (b)) < 0)
#endif
#define KSMALL_LT_DEFINED
#endif

#define KSMALL_CONCAT_(a, b) a ## b
#define KSMALL_CONCAT(a, b) KSMALL_CONCAT_(a, b)
#define KSMALL_FUNC(name) KSMALL_CONCAT(KSMALL_NAME, _##name)

/* This function is adapted from: http://ndevilla.free.fr/median/ */
/* 0 <= kk < n */
static inline KSMALL_TYPE KSMALL_FUNC(ksmall)(size_t n, KSMALL_TYPE arr[], size_t kk)
{
	KSMALL_TYPE *low, *high, *k, *ll, *hh, *mid;
	low = arr; high = arr + n - 1; k = arr + kk;
	for (;;) {
		if (high <= low) return *k;
		if (high == low + 1) {
			if (KSMALL_LT(*high, *low)) KSMALL_SWAP(KSMALL_TYPE, *low, *high);
			return *k;
		}
		mid = low + (high - low) / 2;
		if (KSMALL_LT(*high, *mid)) KSMALL_SWAP(KSMALL_TYPE, *mid, *high);
		if (KSMALL_LT(*high, *low)) KSMALL_SWAP(KSMALL_TYPE, *low, *high);
		if (KSMALL_LT(*low, *mid)) KSMALL_SWAP(KSMALL_TYPE, *mid, *low);
		KSMALL_SWAP(KSMALL_TYPE, *mid, *(low+1));
		ll = low + 1; hh = high;
		for (;;) {
			do ++ll; while (KSMALL_LT(*ll, *low));
			do --hh; while (KSMALL_LT(*low, *hh));
			if (hh < ll) break;
			KSMALL_SWAP(KSMALL_TYPE, *ll, *hh);
		}
		KSMALL_SWAP(KSMALL_TYPE, *low, *hh);
		if (hh <= k) low = ll;
		if (hh >= k) high = hh - 1;
	}
}

#ifdef KSMALL_LT_DEFINED
#undef KSMALL_LT
#undef KSMALL_LT_DEFINED
#endif

#ifdef KSMALL_SWAP_DEFINED
#undef KSMALL_SWAP
#undef KSMALL_SWAP_DEFINED
#endif

#undef KSMALL_CONCAT_
#undef KSMALL_CONCAT
#undef KSMALL_FUNC
