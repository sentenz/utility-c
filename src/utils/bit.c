// SPDX-License-Identifier: Apache-2.0

#include "utility-c/utils/bit.h"

#include <stdlib.h>

bool bit_check(const int x, const int n) {
  return (x >> n) & 1;
}

bool bit_set(int *x, const int n) {
  *x |= (1 << n);

  return bit_check(*x, n);
}

bool bit_unset(int *x, const int n) {
  *x &= ~(1 << n);

  return !bit_check(*x, n);
}

void bit_toggle(int *x, const int n) {
  *x ^= (1 << n);
}
