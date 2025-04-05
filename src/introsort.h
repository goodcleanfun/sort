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

#ifndef INTROSORT_H
#define INTROSORT_H

#include <stdlib.h>
#include <string.h>

typedef struct {
	void *left, *right;
	int depth;
} introsort_stack_t;

#endif

#ifndef INTROSORT_TYPE
#error "Must define INTROSORT_TYPE"
#endif

#ifndef INTROSORT_NAME
#define INTROSORT_NAME_DEFINED
#define INTROSORT_NAME INTROSORT_TYPE
#endif

#ifndef INTROSORT_LT
#if !defined(INTROSORT_STR) && !defined(INTROSORT_AUX_TYPE)
#define INTROSORT_LT(a, b) ((a) < (b))
#elif !defined(INTROSORT_STR) && defined(INTROSORT_AUX_TYPE)
#define INTROSORT_LT(a, b, aux) (((aux)[(a)] < (aux)[(b)]))
#elif defined(INTROSORT_STR) && !defined(INTROSORT_AUX_TYPE)
#define INTROSORT_LT(a, b) (strcmp((a), (b)) < 0)
#else
#define INTROSORT_LT(a, b, aux) (strcmp((aux)[(a)], (aux)[(b)]) < 0)
#endif
#define INTROSORT_LT_DEFINED
#endif

#define INTROSORT_CONCAT_(a, b) a ## b
#define INTROSORT_CONCAT(a, b) INTROSORT_CONCAT_(a, b)
#define INTROSORT_FUNC(name) INTROSORT_CONCAT(INTROSORT_NAME, _##name)


#ifndef INTROSORT_AUX_TYPE
static inline void INTROSORT_FUNC(insertsort)(INTROSORT_TYPE *s, INTROSORT_TYPE *t)
#else
static inline void INTROSORT_FUNC(insertsort)(INTROSORT_TYPE *s, INTROSORT_TYPE *t, INTROSORT_AUX_TYPE *aux)
#endif
{
	INTROSORT_TYPE *i, *j, swap_tmp;
	for (i = s + 1; i < t; ++i)
		#ifndef INTROSORT_AUX_TYPE
		for (j = i; j > s && INTROSORT_LT(*j, *(j-1)); --j)
		#else
		for (j = i; j > s && INTROSORT_LT(*j, *(j-1), aux); --j)
		#endif
		{
			swap_tmp = *j; *j = *(j-1); *(j-1) = swap_tmp;
		}
}

#ifndef INTROSORT_AUX_TYPE
static inline void INTROSORT_FUNC(combsort)(size_t n, INTROSORT_TYPE a[])
#else
static inline void INTROSORT_FUNC(combsort)(size_t n, INTROSORT_TYPE a[], INTROSORT_AUX_TYPE *aux)
#endif
{
	const double shrink_factor = 1.2473309501039786540366528676643;
	int do_swap;
	size_t gap = n;
	INTROSORT_TYPE tmp, *i, *j;
	do {
		if (gap > 2) {
			gap = (size_t)(gap / shrink_factor);
			if (gap == 9 || gap == 10) gap = 11;
		}
		do_swap = 0;
		for (i = a; i < a + n - gap; ++i) {
			j = i + gap;
			#ifndef INTROSORT_AUX_TYPE
			if (INTROSORT_LT(*j, *i))
			#else
			if (INTROSORT_LT(*j, *i, aux))
			#endif 
			{
				tmp = *i; *i = *j; *j = tmp;
				do_swap = 1;
			}
		}
	} while (do_swap || gap > 2);
	if (gap != 1) {
		#ifndef INTROSORT_AUX_TYPE
		INTROSORT_FUNC(insertsort)(a, a + n);
		#else
		INTROSORT_FUNC(insertsort)(a, a + n, aux);
		#endif
	}
}

#ifndef INTROSORT_AUX_TYPE
static inline void INTROSORT_FUNC(introsort)(size_t n, INTROSORT_TYPE a[])
#else
static inline void INTROSORT_FUNC(introsort)(size_t n, INTROSORT_TYPE a[], INTROSORT_AUX_TYPE *aux)
#endif
{
	int d;
	introsort_stack_t *top, *stack;
	INTROSORT_TYPE rp, swap_tmp;
	INTROSORT_TYPE *s, *t, *i, *j, *k;

	if (n < 1) return;
	else if (n == 2) {
		#ifndef INTROSORT_AUX_TYPE
		if (INTROSORT_LT(a[1], a[0])) { swap_tmp = a[0]; a[0] = a[1]; a[1] = swap_tmp; }
		#else
		if (INTROSORT_LT(a[1], a[0], aux)) { swap_tmp = a[0]; a[0] = a[1]; a[1] = swap_tmp; }
		#endif
		return;
	}
	for (d = 2; 1ul<<d < n; ++d);
	stack = (introsort_stack_t*)malloc(sizeof(introsort_stack_t) * ((sizeof(size_t)*d)+2));
	top = stack; s = a; t = a + (n-1); d <<= 1;
	while (1) {
		if (s < t) {
			if (--d == 0) {
				#ifndef INTROSORT_AUX_TYPE
				INTROSORT_FUNC(combsort)(t - s + 1, s);
				#else
				INTROSORT_FUNC(combsort)(t - s + 1, s, aux);
				#endif
				t = s;
				continue;
			}
			i = s; j = t; k = i + ((j-i)>>1) + 1;
			#ifndef INTROSORT_AUX_TYPE
			if (INTROSORT_LT(*k, *i)) {
				if (INTROSORT_LT(*k, *j)) k = j;
			} else k = INTROSORT_LT(*j, *i)? i : j;
			#else
			if (INTROSORT_LT(*k, *i, aux)) {
				if (INTROSORT_LT(*k, *j, aux)) k = j;
			} else k = INTROSORT_LT(*j, *i, aux)? i : j;
			#endif
			rp = *k;
			if (k != t) { swap_tmp = *k; *k = *t; *t = swap_tmp; }
			for (;;) {
				#ifndef INTROSORT_AUX_TYPE
				do ++i; while (INTROSORT_LT(*i, rp));
				do --j; while (i <= j && INTROSORT_LT(rp, *j));
				#else
				do ++i; while (INTROSORT_LT(*i, rp, aux));
				do --j; while (i <= j && INTROSORT_LT(rp, *j, aux));
				#endif
				if (j <= i) break;
				swap_tmp = *i; *i = *j; *j = swap_tmp;
			}
			swap_tmp = *i; *i = *t; *t = swap_tmp;
			if (i-s > t-i) {
				if (i-s > 16) { top->left = s; top->right = i-1; top->depth = d; ++top; }
				s = t-i > 16? i+1 : t;
			} else {
				if (t-i > 16) { top->left = i+1; top->right = t; top->depth = d; ++top; }
				t = i-s > 16? i-1 : s;
			}
		} else {
			if (top == stack) {
				free(stack);
				#ifndef INTROSORT_AUX_TYPE
				INTROSORT_FUNC(insertsort)(a, a+n);
				#else
				INTROSORT_FUNC(insertsort)(a, a+n, aux);
				#endif
				return;
			} else { --top; s = (INTROSORT_TYPE*)top->left; t = (INTROSORT_TYPE*)top->right; d = top->depth; }
		}
	}
}

#undef INTROSORT_CONCAT_
#undef INTROSORT_CONCAT
#undef INTROSORT_FUNC

#ifdef INTROSORT_NAME_DEFINED
#undef INTROSORT_NAME
#undef INTROSORT_NAME_DEFINED
#endif

#ifdef INTROSORT_LT_DEFINED
#undef INTROSORT_LT
#undef INTROSORT_LT_DEFINED
#endif
