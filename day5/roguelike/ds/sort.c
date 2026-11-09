#include "sort.h"

static int cmp_price(const void *a, const void *b)
{
    const BagRow *x = a;
    const BagRow *y = b;
    return (x->price > y->price) - (x->price < y->price);
}

static int cmp_rare(const void *a, const void *b)
{
    const BagRow *x = a;
    const BagRow *y = b;
    return (x->rare < y->rare) - (x->rare > y->rare); /* 高稀有在前 */
}

static int cmp_weight(const void *a, const void *b)
{
    const BagRow *x = a;
    const BagRow *y = b;
    return (x->weight > y->weight) - (x->weight < y->weight);
}

void sort_by_price_qsort(BagRow *a, size_t n)
{
    qsort(a, n, sizeof *a, cmp_price);
}

void sort_by_rare(BagRow *a, size_t n)
{
    qsort(a, n, sizeof *a, cmp_rare);
}

void sort_by_weight(BagRow *a, size_t n)
{
    qsort(a, n, sizeof *a, cmp_weight);
}

static void merge(BagRow *a, BagRow *tmp, size_t l, size_t m, size_t r)
{
    size_t i = l, j = m, k = l;
    while (i < m && j < r) {
        if (a[i].price < a[j].price ||
            (a[i].price == a[j].price && a[i].order <= a[j].order)) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    while (i < m) {
        tmp[k++] = a[i++];
    }
    while (j < r) {
        tmp[k++] = a[j++];
    }
    for (size_t t = l; t < r; t++) {
        a[t] = tmp[t];
    }
}

static void msort(BagRow *a, BagRow *tmp, size_t l, size_t r)
{
    if (r - l <= 1) {
        return;
    }
    size_t m = l + (r - l) / 2;
    msort(a, tmp, l, m);
    msort(a, tmp, m, r);
    merge(a, tmp, l, m, r);
}

void sort_by_price_mergesort(BagRow *a, size_t n)
{
    BagRow *tmp = malloc(n * sizeof *tmp);
    if (!tmp) {
        return;
    }
    msort(a, tmp, 0, n);
    free(tmp);
}
