// SPDX-License-Identifier: Apache-2.0

#include "utility-c/util.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

bool util_isFloat(const double n) {
  if (roundf(n) == n) {
    return false;
  }

  return true;
}

int util_countDigits(const int n) {
  if (n == 0) {
    return 1;
  }

  return floor(log10(abs(n))) + 1;
}

int util_countFloat(const double n) {
  if (!util_isFloat(n)) {
    return 0;
  }

  int count = 0;
  while (((float)pow(10, count)) * n != (int)(((float)pow(10, count)) * n)) {
    count++;
  }

  return count + 1;
}

int util_countNumber(const double n) {
  if (util_isFloat(n)) {
    return util_countFloat(n);
  }

  return util_countDigits(n);
}

bool util_approximatelyEqual(float a, float b, float epsilon) {
  return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool util_essentiallyEqual(float a, float b, float epsilon) {
  return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool util_definitelyGreaterThan(float a, float b, float epsilon) {
  return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool util_definitelyLessThan(float a, float b, float epsilon) {
  return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}
