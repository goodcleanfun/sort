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

#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#endif

#ifndef HEAPSORT_TYPE
#error "Must define HEAPSORT_TYPE"
#endif

#ifndef HEAPSORT_NAME
#define HEAPSORT_NAME_DEFINED
#define HEAPSORT_NAME HEAPSORT_TYPE
#endif

#ifndef HEAPSORT_LT
#if !defined(HEAPSORT_STR) && !defined(HEAPSORT_AUX_TYPE)
#define HEAPSORT_LT(a, b) ((a) < (b))
#elif !defined(HEAPSORT_STR) && defined(HEAPSORT_AUX_TYPE)
#define HEAPSORT_LT(a, b, aux) (((aux)[(a)] < (aux)[(b)]))
#elif defined(HEAPSORT_STR) && !defined(HEAPSORT_AUX_TYPE)
#define HEAPSORT_LT(a, b) (strcmp((a), (b)) < 0)
#else
#define HEAPSORT_LT(a, b, aux) (strcmp((aux)[(a)], (aux)[(b)]) < 0)
#endif
#define HEAPSORT_LT_DEFINED
#endif

#define HEAPSORT_CONCAT_(a, b) a ## b
#define HEAPSORT_CONCAT(a, b) HEAPSORT_CONCAT_(a, b)
#define HEAPSORT_FUNC(name) HEAPSORT_CONCAT(HEAPSORT_NAME, _##name)

static inline void HEAPSORT_FUNC(heapadjust_direction)(size_t i, size_t n, HEAPSORT_TYPE l[], bool reverse)
{
	size_t k = i;
	HEAPSORT_TYPE tmp = l[i];
	while ((k = (k << 1) + 1) < n) {
		#ifndef HEAPSORT_AUX_TYPE
		if (k != n - 1 && (HEAPSORT_LT(l[k], l[k+1]) ^ reverse)) ++k;
		if (HEAPSORT_LT(l[k], tmp) ^ reverse) break;
		#else
		if (k != n - 1 && (HEAPSORT_LT(l[k], l[k+1], aux) ^ reverse)) ++k;
		if (HEAPSORT_LT(l[k], tmp, aux) ^ reverse) break;
		#endif
		l[i] = l[k]; i = k;
	}
	l[i] = tmp;
}

static inline void HEAPSORT_FUNC(heapmake_direction)(size_t lsize, HEAPSORT_TYPE l[], bool reverse)
{
	size_t i;
	for (i = (lsize >> 1) - 1; i != (size_t)(-1); --i)
		HEAPSORT_FUNC(heapadjust_direction)(i, lsize, l, reverse);
}

static inline void HEAPSORT_FUNC(heapsort_direction)(size_t lsize, HEAPSORT_TYPE l[], bool reverse)
{
	size_t i;
	HEAPSORT_FUNC(heapmake_direction)(lsize, l, reverse);
	for (i = lsize - 1; i > 0; --i) {
		HEAPSORT_TYPE tmp;
		tmp = *l; *l = l[i]; l[i] = tmp; 
		HEAPSORT_FUNC(heapadjust_direction)(0, i, l, reverse);
	}
}

// Original functions without direction parameter (ascending order)
static inline void HEAPSORT_FUNC(heapadjust)(size_t i, size_t n, HEAPSORT_TYPE l[])
{
	HEAPSORT_FUNC(heapadjust_direction)(i, n, l, false);
}

static inline void HEAPSORT_FUNC(heapmake)(size_t lsize, HEAPSORT_TYPE l[])
{
	HEAPSORT_FUNC(heapmake_direction)(lsize, l, false);
}

static inline void HEAPSORT_FUNC(heapsort)(size_t lsize, HEAPSORT_TYPE l[])
{
	HEAPSORT_FUNC(heapsort_direction)(lsize, l, false);
}

// Reverse versions (descending order)
static inline void HEAPSORT_FUNC(heapadjust_reverse)(size_t i, size_t n, HEAPSORT_TYPE l[])
{
	HEAPSORT_FUNC(heapadjust_direction)(i, n, l, true);
}

static inline void HEAPSORT_FUNC(heapmake_reverse)(size_t lsize, HEAPSORT_TYPE l[])
{
	HEAPSORT_FUNC(heapmake_direction)(lsize, l, true);
}

static inline void HEAPSORT_FUNC(heapsort_reverse)(size_t lsize, HEAPSORT_TYPE l[])
{
	HEAPSORT_FUNC(heapsort_direction)(lsize, l, true);
}

#undef HEAPSORT_CONCAT_
#undef HEAPSORT_CONCAT
#undef HEAPSORT_FUNC

#ifdef HEAPSORT_NAME_DEFINED
#undef HEAPSORT_NAME
#undef HEAPSORT_NAME_DEFINED
#endif

#ifdef HEAPSORT_LT_DEFINED
#undef HEAPSORT_LT
#endif
#undef HEAPSORT_LT_DEFINED
