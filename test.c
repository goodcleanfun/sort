#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define HEAPSORT_TYPE int
#include "heapsort.h"

#define INTROSORT_TYPE int
#include "introsort.h"

#define MERGESORT_TYPE int
#include "mergesort.h"

#define RADIX_SORT_TYPE int
#include "radixsort.h"

#define SHUFFLE_TYPE int
#include "shuffle.h"

#include "greatest/greatest.h"

TEST test_heapsort(void) {
    int a[] = {9, 4, 3, 5, 2, 1, 6, 7, 10, 8, 16, 12, 13, 15, 14, 11};
    size_t n = sizeof(a) / sizeof(int);
    heapmake_int(n, a);
    heapsort_int(n, a);
    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(a[i], i + 1);
    }
    PASS();
}

TEST test_introsort(void) {
    int a[] = {9, 4, 3, 5, 2, 1, 6, 7, 10, 8, 16, 12, 13, 15, 14, 11};
    size_t n = sizeof(a) / sizeof(int);
    introsort_int(n, a);
    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(a[i], i + 1);
    }
    PASS();
}

TEST test_mergesort(void) {
    int a[] = {9, 4, 3, 5, 2, 1, 6, 7, 10, 8, 16, 12, 13, 15, 14, 11};
    size_t n = sizeof(a) / sizeof(int);
    mergesort_int(n, a, NULL);
    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(a[i], i + 1);
    }
    PASS();
}

TEST test_radix_sort(void) {
    int a[] = {9, 4, 3, 5, 2, 1, 6, 7, 10, 8, 16, 12, 13, 15, 14, 11};
    size_t n = sizeof(a) / sizeof(int);
    radix_sort_int(a, a + n);

    for (size_t i = 0; i < n; i++) {
        ASSERT_EQ(a[i], i + 1);
    }

    PASS();
}

TEST test_shuffle(void) {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 
               10, 11, 12, 13, 14, 15, 16};

    int_shuffle(sizeof(a) / sizeof(int), a);

    PASS();
}


GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_heapsort);
    RUN_TEST(test_introsort);
    RUN_TEST(test_mergesort);
    RUN_TEST(test_radix_sort);
    RUN_TEST(test_shuffle);

    GREATEST_MAIN_END();        /* display results */
}
