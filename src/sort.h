#ifndef ARRAY_SORT_H
#define ARRAY_SORT_H

#include <stdbool.h>
#include <stdlib.h>

#endif

#ifndef ARRAY_NAME
#error "ARRAY_NAME must be defined"
#endif

#ifndef ARRAY_TYPE
#error "ARRAY_TYPE must be defined"
#endif

#define ARRAY_SORT_CONCAT_(a, b) a ## b
#define ARRAY_SORT_CONCAT(a, b) ARRAY_SORT_CONCAT_(a, b)
#define ARRAY_SORT_FUNC(name) ARRAY_SORT_CONCAT(ARRAY_NAME, _##name)

#define ARRAY_INDEX_NAME ARRAY_SORT_CONCAT(ARRAY_NAME, _index)
#define ARRAY_SORT_INDEX_FUNC(name) ARRAY_SORT_CONCAT(ARRAY_INDEX_NAME, _##name)


#define INTROSORT_NAME ARRAY_NAME
#define INTROSORT_TYPE ARRAY_TYPE
#ifdef ARRAY_SORT_LT
#define INTROSORT_LT(a, b) ARRAY_SORT_LT(a, b)
#endif
#include "introsort.h"
#ifdef INTROSORT_LT
#undef INTROSORT_LT
#endif
#undef INTROSORT_TYPE
#undef INTROSORT_NAME

static inline bool ARRAY_SORT_FUNC(sort)(ARRAY_TYPE *array, size_t n) {
    if (array == NULL) return false;
    ARRAY_SORT_FUNC(introsort)(n, array);
    return true;
}

static inline bool ARRAY_SORT_FUNC(sort_reverse)(ARRAY_TYPE *array, size_t n) {
    if (array == NULL) return false;
    ARRAY_SORT_FUNC(introsort_reverse)(n, array);
    return true;
}

#define INTROSORT_NAME ARRAY_INDEX_NAME
#define INTROSORT_TYPE size_t
#define INTROSORT_AUX_TYPE ARRAY_TYPE
#include "introsort.h"
#undef INTROSORT_NAME
#undef INTROSORT_TYPE
#undef INTROSORT_AUX_TYPE

static inline bool ARRAY_SORT_FUNC(argsort)(ARRAY_TYPE *array, size_t n, size_t *indices) {
    if (indices == NULL) return false;
    for (size_t i = 0; i < n; i++) {
        indices[i] = i;
    }
    ARRAY_SORT_INDEX_FUNC(introsort)(n, indices, array);
    return true;
}

static inline bool ARRAY_SORT_FUNC(argsort_reverse)(ARRAY_TYPE *array, size_t n, size_t *indices) {
    if (indices == NULL) return false;
    for (size_t i = 0; i < n; i++) {
        indices[i] = i;
    }
    ARRAY_SORT_INDEX_FUNC(introsort_reverse)(n, indices, array);
    return true;
}

#undef ARRAY_INDEX_NAME
#undef ARRAY_SORT_FUNC
#undef ARRAY_SORT_INDEX_FUNC
