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

#ifndef AC_KSORT_H
#define AC_KSORT_H

#include <stdlib.h>
#include <string.h>

#include "common.h"

#define SHUFFLE_INIT(name, type_t, __sort_lt)							\
	static inline void ks_shuffle_##name(size_t n, type_t a[])			\
	{																	\
		int i, j;														\
		for (i = n; i > 1; --i) {										\
			type_t tmp;													\
			j = (int)(drand48() * i);									\
			tmp = a[j]; a[j] = a[i-1]; a[i-1] = tmp;					\
		}																\
	}

#define ks_shuffle(name, n, a) ks_shuffle_##name(n, a)

#define SHUFFLE_INIT_GENERIC(type_t) SHUFFLE_INIT(type_t, type_t, ks_lt_generic)
#define SHUFFLE_INIT_STR SHUFFLE_INIT(str, ksstr_t, ks_lt_str)

#endif
