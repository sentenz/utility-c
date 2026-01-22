// SPDX-License-Identifier: Apache-2.0

#include "utility-c/uri.h"

#include <uriparser/Uri.h>

#define PARSER(x) (int)((x).afterLast - (x).first), ((x).first)

static int internal_compose(const char *str, UriUriA *uri) {
  const char *err;

  return uriParseSingleUriA(uri, str, &err);
}

static char *internal_range(const UriTextRangeA *range) {
  if (range->first == NULL || range->afterLast == NULL) {
    return NULL;
  }

  return strndup(range->first, (size_t)(range->afterLast - range->first));
}

static char *internal_path(const UriPathSegmentA *path) {
  if (path == NULL) {
    return NULL;
  }

  return strdup(path->text.first);
}

char *uri_scheme(const char *str) {
  UriUriA uri;
  int retval = internal_compose(str, &uri);
  if (retval != URI_SUCCESS) {
    return NULL;
  }

  char *content = internal_range(&uri.scheme);
  if (content == NULL) {
    uriFreeUriMembersA(&uri);

    return NULL;
  }

  uriFreeUriMembersA(&uri);

  return content;
}

char *uri_host(const char *str) {
  UriUriA uri;
  int retval = internal_compose(str, &uri);
  if (retval != URI_SUCCESS) {
    return NULL;
  }

  char *content = internal_range(&uri.hostText);
  if (content == NULL) {
    uriFreeUriMembersA(&uri);

    return NULL;
  }

  uriFreeUriMembersA(&uri);

  return content;
}

char *uri_port(const char *str) {
  UriUriA uri;
  int retval = internal_compose(str, &uri);
  if (retval != URI_SUCCESS) {
    return NULL;
  }

  char *content = internal_range(&uri.portText);
  if (content == NULL) {
    uriFreeUriMembersA(&uri);

    return NULL;
  }

  uriFreeUriMembersA(&uri);

  return content;
}

char *uri_user(const char *str) {
  UriUriA uri;
  int retval = internal_compose(str, &uri);
  if (retval != URI_SUCCESS) {
    return NULL;
  }

  char *content = internal_range(&uri.userInfo);
  if (content == NULL) {
    uriFreeUriMembersA(&uri);

    return NULL;
  }

  uriFreeUriMembersA(&uri);

  return content;
}

char *uri_query(const char *str) {
  UriUriA uri;
  int retval = internal_compose(str, &uri);
  if (retval != URI_SUCCESS) {
    return NULL;
  }

  char *content = internal_range(&uri.query);
  if (content == NULL) {
    uriFreeUriMembersA(&uri);

    return NULL;
  }

  uriFreeUriMembersA(&uri);

  return content;
}

char *uri_fragment(const char *str) {
  UriUriA uri;
  int retval = internal_compose(str, &uri);
  if (retval != URI_SUCCESS) {
    return NULL;
  }

  char *content = internal_range(&uri.fragment);
  if (content == NULL) {
    uriFreeUriMembersA(&uri);

    return NULL;
  }

  uriFreeUriMembersA(&uri);

  return content;
}

char *uri_path(const char *str) {
  // XXX(Sentenz) Trailing slashes after the domain name should normally not matter `domain.com =
  // domain.com/`, however uriparser resolves the path of a `domain.com/` with trailing slashes with
  // "X" instead of a NULL pointer

  UriUriA uri;
  int retval = internal_compose(str, &uri);
  if (retval != URI_SUCCESS) {
    return NULL;
  }

  char *content = internal_path(uri.pathHead);
  if (content == NULL) {
    uriFreeUriMembersA(&uri);

    return NULL;
  }

  uriFreeUriMembersA(&uri);

  return content;
}
