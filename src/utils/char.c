// SPDX-License-Identifier: Apache-2.0

#include "utility-c/char.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "utility-c/util.h"

bool char_isEmpty(const char *str) {
  return str == NULL || str[0] == '\0';
}

bool char_isValid(const char *str) {
  if (char_isEmpty(str)) {
    return false;
  }

  for (size_t i = 0; i < strlen(str); ++i) {
    if (isascii(str[i]) == 0) {
      return false;
    }
  }

  return true;
}

bool char_areEqual(const char *str1, const char *str2) {
  if (char_isEmpty(str1) || char_isEmpty(str2)) {
    return false;
  }

  return strcmp(str1, str2) == 0;
}

int char_toInteger(const char *str) {
  if (char_isEmpty(str)) {
    return 0;
  }

  return strtol(str, NULL, 10);
}

char *char_fromInteger(const int n) {
  int digits = util_countDigits(n);
  if (n < 0) {
    // Represents a negative number `-` sign
    digits += 1;
  }

  size_t len = digits + 1;

  char *content = (char *)malloc(sizeof(char) * len);
  if (content == NULL) {
    return NULL;
  }

  snprintf(content, len, "%d", n);

  return content;
}

double char_toFloat(const char *str) {
  if (char_isEmpty(str)) {
    return 0;
  }

  return strtod(str, NULL);
}

char *char_fromFloat(const double n) {
  int digits = util_countFloat(n);
  if (digits == 0) {
    return NULL;
  }

  if (n < 0) {
    // Representsa dot notation `.`  floating-point number and a negative number `-` sign
    digits += 2;
  } else {
    // Represents a dot notation `.` floating-point number
    digits += 1;
  }

  size_t len = digits + 1;

  char *content = (char *)malloc(sizeof(char) * len);
  if (content == NULL) {
    return NULL;
  }

  snprintf(content, len, "%f", n);

  return content;
}

char *char_fromNumber(const double n) {
  if (util_isFloat(n)) {
    return char_fromFloat(n);
  }

  return char_fromInteger(n);
}

char *char_concats(const char *str, ...) {
  // XXX(Sentenz) Refactor this function based on the strcat() function

  size_t allocated = 100;
  char *content    = (char *)malloc(allocated);
  if (content == NULL) {
    return NULL;
  }

  char *newp;
  char *concats;
  const char *tmp;

  va_list ap;
  va_start(ap, str);

  concats = content;
  for (tmp = str; tmp != NULL; tmp = va_arg(ap, const char *)) {
    size_t len = strlen(tmp);

    // Resize the allocated memory if necessary
    if (concats + len + 1 > content + allocated) {
      allocated = (allocated + len) * 2;
      newp      = (char *)realloc(content, allocated);
      if (newp == NULL) {
        free(content);
        va_end(ap);

        return NULL;
      }
      concats = newp + (concats - content);
      content = newp;
    }

#ifdef S_POSIX
    concats = mempcpy(concats, tmp, len);
#else
    concats = (char *)memcpy(concats, tmp, len) + len;
#endif  // S_POSIX
  }

  // Terminate the content string
  *concats++ = '\0';

  // Resize memory to the optimal size
  newp = realloc(content, concats - content);
  if (newp != NULL) {
    content = newp;
  }

  va_end(ap);

  return content;
}

char *char_trimSpace(const char *str) {
  if (char_isEmpty(str)) {
    return NULL;
  }

  size_t len = strlen(str);

  while (isspace(str[len - 1])) {
    --len;
  }

  while (*str && isspace(*str)) {
    ++str, --len;
  }

  if (char_isEmpty(str)) {
    return NULL;
  }

  return strndup(str, len);
}

char *char_trimNonAlphanum(const char *str) {
  if (char_isEmpty(str)) {
    return NULL;
  }

  size_t len = strlen(str);

  while (!isalnum(str[len - 1])) {
    --len;
  }

  while (*str && !isalnum(*str)) {
    ++str, --len;
  }

  if (char_isEmpty(str)) {
    return NULL;
  }

  return strndup(str, len);
}

char *char_toUppercase(const char *str) {
  if (char_isEmpty(str)) {
    return NULL;
  }

  char content[strlen(str) + 1];
  memset(content, '\0', strlen(str) + 1);

  for (size_t i = 0; i < strlen(str); ++i) {
    content[i] = toupper(str[i]);
  }

  return strndup(content, strlen(content));
}

char *char_toLowercase(const char *str) {
  if (char_isEmpty(str)) {
    return NULL;
  }

  char content[strlen(str) + 1];
  memset(content, '\0', strlen(str) + 1);

  for (size_t i = 0; i < strlen(str); ++i) {
    content[i] = tolower(str[i]);
  }

  return strndup(content, strlen(content));
}

size_t char_occurrences(const char *str, const char *sub) {
  if (char_isEmpty(str) || char_isEmpty(sub)) {
    return 0;
  }

  size_t count = 0;
  char *pos    = (char *)str;

  while ((pos = strstr(pos, sub))) {
    pos += strlen(sub);
    count++;
  }

  return count;
}

char *char_replace(const char *str, const char *pre, const char *post, int count) {
  if (char_isEmpty(str) || char_isEmpty(pre) || post == NULL) {
    return NULL;
  }

  size_t occur = char_occurrences(str, pre);
  if (occur == 0 || count == 0) {
    return strndup(str, strlen(str));
  }

  int len       = (strlen(str) - (strlen(pre) * occur) + strlen(post) * occur) + 1;
  char *content = (char *)malloc(sizeof(char) * len);
  if (content == NULL) {
    return NULL;
  }
  memset(content, '\0', len);

  char *current;
  char *pos = (char *)str;

  while ((current = strstr(pos, pre))) {
    int len = current - pos;
    strncat(content, pos, len);
    strncat(content, post, strlen(post));
    pos = current + strlen(pre);

    if (--occur == 0 || --count == 0) {
      break;
    }
  }

  if (pos != (str + strlen(str))) {
    strncat(content, pos, (str - pos));
  }

  return content;
}

bool char_contains(const char *str, char const *sub) {
  if (char_isEmpty(str) || char_isEmpty(sub)) {
    return false;
  }

  if (strstr(str, sub) == NULL) {
    return false;
  }

  return true;
}

void char_free(char *str) {
  if (str == NULL) {
    return;
  }

  free(str);
}
