#include "search.h"

size_t lower_bound_price(const BagRow *a, size_t n, int price)
{
    size_t l = 0, r = n;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if (a[m].price < price) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    return l;
}

int linear_find_id(const BagRow *a, size_t n, int id)
{
    for (size_t i = 0; i < n; i++) {
        if (a[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}
