// SPDX-License-Identifier: Apache-2.0

#include "utility-c/utils/regex.h"

#include <string.h>
#include <stdlib.h>

#if defined(S_PCRE)
  #include <pcre.h>
#elif defined(S_PCRE2)
  #include <pcre2.h>
#else
  #include <regex.h>
#endif

#include "utility-c/utils/char.h"

#ifdef S_PCRE

static int internal_pcre_match(const char *pattern, const char *str, const bool repeat) {
  pcre *re                  = NULL;
  const char *error         = NULL;
  char *subject             = NULL;
  unsigned char *name_table = NULL;
  unsigned int option_bits;
  int erroffset;
  int crlf_is_newline;
  int namecount;
  int name_entry_size;
  int ovector[OVECCOUNT];
  int subject_length;
  int rc;
  int utf8;

  subject        = (char *)str;
  subject_length = (int)strlen(subject);

  /* Compile the regular expression pattern, and handle and errors that are detected. */
  re = pcre_compile(pattern,    /* the pattern */
                    0,          /* default options */
                    &error,     /* for error message */
                    &erroffset, /* for error offset */
                    NULL);      /* use default character tables */

  /* Compilation failed: print the error message and exit */

  if (re == NULL) {
    return -1;
  }

  /* If the compilation succeeded, we call PCRE again, in order to do a pattern match against the
   * subject string. This does just ONE match. If further matching is needed, it will be done below.
   */

  rc = pcre_exec(re,             /* the compiled pattern */
                 NULL,           /* no extra data - we didn't study the pattern */
                 subject,        /* the subject string */
                 subject_length, /* the length of the subject */
                 0,              /* start at offset 0 in the subject */
                 0,              /* default options */
                 ovector,        /* output vector for substring information */
                 OVECCOUNT);     /* number of elements in the output vector */

  /* Matching failed: handle error cases */
  if (rc < 0) {
    switch (rc) {
      case PCRE_ERROR_NOMATCH: {
        break;
      }
      default: {
        break;
      }
    }

    /* Release memory used for the compiled pattern */
    pcre_free(re);

    return -1;
  }

  /*  We have found the first match within the subject string. If the output vector wasn't big
   * enough, say so. Then output any substrings that were captured. */

  /* The output vector wasn't big enough */
  if (rc == 0) {
    rc = OVECCOUNT / 3;
  }

  /* See if there are any named substrings, and if so, show them by name. First we have to extract
   * the count of named parentheses from the pattern. */

  (void)pcre_fullinfo(re,                  /* the compiled pattern */
                      NULL,                /* no extra data - we didn't study the pattern */
                      PCRE_INFO_NAMECOUNT, /* number of named substrings */
                      &namecount);         /* where to put the answer */

  if (namecount > 0) {
    unsigned char *tabptr;

    /* Before we can access the substrings, we must extract the table for
    translating names to numbers, and the size of each entry in the table. */

    (void)pcre_fullinfo(re,                      /* the compiled pattern */
                        NULL,                    /* no extra data - we didn't study the pattern */
                        PCRE_INFO_NAMETABLE,     /* address of the table */
                        &name_table);            /* where to put the answer */

    (void)pcre_fullinfo(re,                      /* the compiled pattern */
                        NULL,                    /* no extra data - we didn't study the pattern */
                        PCRE_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
                        &name_entry_size);       /* where to put the answer */

    /* Now we can scan the table and, for each entry, print the number, the
    name, and the substring itself. */
    tabptr = name_table;

    for (int i = 0; i < namecount; i++) {
      int n = (tabptr[0] << 8) | tabptr[1];
      tabptr += name_entry_size;
    }
  }

  if (!repeat) {
    /* Release the memory used for the compiled pattern */
    pcre_free(re);

    /* Finish unless -g was given */
    return 0;
  }

  /* Before running the loop, check for UTF-8 and whether CRLF is a valid newline sequence. First,
   * find the options with which the regex was compiled; extract the UTF-8 state, and mask off all
   * but the newline options. */

  (void)pcre_fullinfo(re, NULL, PCRE_INFO_OPTIONS, &option_bits);
  utf8 = option_bits & PCRE_UTF8;
  option_bits &= PCRE_NEWLINE_CR | PCRE_NEWLINE_LF | PCRE_NEWLINE_CRLF | PCRE_NEWLINE_ANY |
                 PCRE_NEWLINE_ANYCRLF;

  /* If no newline options were set, find the default newline convention from
  the build configuration. */

  if (option_bits == 0) {
    int d;
    (void)pcre_config(PCRE_CONFIG_NEWLINE, &d);
    /* Note that these values are always the ASCII ones, even in
    EBCDIC environments. CR = 13, NL = 10. */
    option_bits = (d == 13)               ? PCRE_NEWLINE_CR
                  : (d == 10)             ? PCRE_NEWLINE_LF
                  : (d == (13 << 8 | 10)) ? PCRE_NEWLINE_CRLF
                  : (d == -2)             ? PCRE_NEWLINE_ANYCRLF
                  : (d == -1)             ? PCRE_NEWLINE_ANY
                                          : 0;
  }

  /* See if CRLF is a valid newline sequence. */

  crlf_is_newline = option_bits == PCRE_NEWLINE_ANY || option_bits == PCRE_NEWLINE_CRLF ||
                    option_bits == PCRE_NEWLINE_ANYCRLF;

  /* Loop for second and subsequent matches */

  for (;;) {
    int options      = 0;          /* Normally no options */
    int start_offset = ovector[1]; /* Start at end of previous match */

    /* If the previous match was for an empty string, we are finished if we are
    at the end of the subject. Otherwise, arrange to run another match at the
    same point to see if a non-empty match can be found. */

    if (ovector[0] == ovector[1]) {
      if (ovector[0] == subject_length) {
        break;
      }
      options = PCRE_NOTEMPTY_ATSTART | PCRE_ANCHORED;
    }

    /* Run the next matching operation */

    rc = pcre_exec(re,             /* the compiled pattern */
                   NULL,           /* no extra data - we didn't study the pattern */
                   subject,        /* the subject string */
                   subject_length, /* the length of the subject */
                   start_offset,   /* starting offset in the subject */
                   options,        /* options */
                   ovector,        /* output vector for substring information */
                   OVECCOUNT);     /* number of elements in the output vector */

    /* This time, a result of NOMATCH isn't an error. If the value in "options" is zero, it just
    means we have found all possible matches, so the loop ends. Otherwise, it means we have failed
    to find a non-empty-string match at a point where there was a previous empty-string match. In
    this case, we do what Perl does: advance the matching position by one character, and continue.
    We do this by setting the "end of previous match" offset, because that is picked up at the top
    of the loop as the point at which to start again.

    There are two complications: (a) When CRLF is a valid newline sequence, and the current position
    is just before it, advance by an extra byte. (b) Otherwise we must ensure that we skip an entire
    UTF-8 character if we are in UTF-8 mode. */

    if (rc == PCRE_ERROR_NOMATCH) {
      if (options == 0) {
        /* All matches found */
        break;
      }
      /* Advance one byte */
      ovector[1] = start_offset + 1;
      /* If CRLF is newline & we are at CRLF */
      if (crlf_is_newline && start_offset < subject_length - 1 && subject[start_offset] == '\r' &&
          subject[start_offset + 1] == '\n') {
        /* Advance by one more */
        ovector[1] += 1;
        /* Otherwise, ensure we advance a whole UTF-8 character */
      } else if (utf8) {
        while (ovector[1] < subject_length) {
          if ((subject[ovector[1]] & 0xc0) != 0x80) {
            break;
          }
          ovector[1] += 1;
        }
      }
      /* Go round the loop again */
      continue;
    }

    /* Other matching errors are not recoverable. */

    if (rc < 0) {
      /* Release memory used for the compiled pattern */
      pcre_free(re);

      return -1;
    }

    /* The match succeeded, but the output vector wasn't big enough. */
    if (rc == 0) {
      rc = OVECCOUNT / 3;
    }

    /* As before, show substrings stored in the output vector by number, and
    then also any named substrings. */
    if (namecount > 0) {
      unsigned char *tabptr = name_table;

      for (int i = 0; i < namecount; i++) {
        int n = (tabptr[0] << 8) | tabptr[1];
        tabptr += name_entry_size;
      }
    }
  } /* End of loop to find second and subsequent matches */

  /* Release memory used for the compiled pattern */
  pcre_free(re);

  return 0;
}

static char *internal_prce_between(const char *str,
                                                  const char *leftPattern,
                                                  const char *rightPattern) {
  pcre *re          = NULL;
  char *pattern     = NULL;
  char *subject     = NULL;
  const char *error = NULL;
  char *retval      = NULL;
  int erroffset;
  int ovector[OVECCOUNT];
  int subject_length;
  int rc;

  /* After the options, we require exactly two arguments, which are the pattern, and the subject
   * string. */

  pattern        = char_concats("(?<=", leftPattern, ").*(?=", rightPattern, ")", NULL);
  subject        = (char *)str;
  subject_length = (int)strlen(subject);

  /* Compile the regular expression pattern, and handle and errors that are detected. */

  re = pcre_compile(pattern,    /* the pattern */
                    0,          /* default options */
                    &error,     /* for error message */
                    &erroffset, /* for error offset */
                    NULL);      /* use default character tables */
  if (NULL != pattern) {
    free(pattern);
  }

  /* Compilation failed */
  if (re == NULL) {
    return NULL;
  }

  /* If the compilation succeeded, we call PCRE again, in order to do a pattern match against the
   * subject string. This does just ONE match. If further matching is needed, it will be done below.
   */

  rc = pcre_exec(re,             /* the compiled pattern */
                 NULL,           /* no extra data - we didn't study the pattern */
                 subject,        /* the subject string */
                 subject_length, /* the length of the subject */
                 0,              /* start at offset 0 in the subject */
                 0,              /* default options */
                 ovector,        /* output vector for substring information */
                 OVECCOUNT);     /* number of elements in the output vector */

  /* Matching failed: handle error cases */
  if (rc < 0) {
    switch (rc) {
      case PCRE_ERROR_NOMATCH:
        break;
      default:
        break;
    }

    /* Release memory used for the compiled pattern */
    pcre_free(re);

    return NULL;
  }

  /* We have found the first match within the subject string. If the output vector wasn't big
   * enough, say so. Then output any substrings that were captured. */

  /* The output vector wasn't big enough */
  if (rc == 0) {
    rc = OVECCOUNT / 3;
  }

  char *substring_start = subject + ovector[0];
  int substring_length  = ovector[1] - ovector[0];

  retval = malloc(substring_length + 1);
  snprintf(retval, substring_length + 1, "%s", substring_start);

  /* Cleanup */
  pcre_free(re);

  return retval;
}

static char *internal_prce_find(const char *pattern, const char *str) {
  const char *errorptr = NULL;
  int erroffset;

  pcre *compile = pcre_compile(pattern, 0, &errorptr, &erroffset, NULL);
  if (compile == NULL) {
    return NULL;
  }

  int ovector[OVECCOUNT];

  int err = pcre_exec(compile, NULL, str, (int)strlen(str), 0, 0, ovector, OVECCOUNT);
  if (err < 0) {
    switch (err) {
      case PCRE_ERROR_NOMATCH:
        break;
      default:
        break;
    }

    pcre_free(compile);

    return NULL;
  }

  if (err == 0) {
    // The output vector wasn't big enough
    err = OVECCOUNT / 3;
  }

  char *start = str + ovector[0];
  int len     = ovector[1] - ovector[0] + 1;

  char *match = (char *)malloc(sizeof(char) * len);
  if (match == NULL) {
    pcre_free(compile);

    return NULL;
  }

  snprintf(match, len, "%s", start);

  pcre_free(compile);

  return match;
}

#elif S_PCRE2

static int internal_pcre2_match(const char *regex, const char *match, const bool repeat) {
  pcre2_code *re;
  PCRE2_SPTR pattern; /* PCRE2_SPTR is a pointer to unsigned code units of */
  PCRE2_SPTR subject; /* the appropriate width (8, 16, or 32 bits). */
  PCRE2_SPTR name_table;

  int crlf_is_newline;
  int errornumber;
  int namecount;
  int name_entry_size;
  int rc;
  int utf8;

  uint32_t option_bits;
  uint32_t newline;

  PCRE2_SIZE erroroffset;
  PCRE2_SIZE *ovector;

  size_t subject_length;
  pcre2_match_data *match_data;

  /* As pattern and subject are char arguments, they can be straightforwardly
   * cast to PCRE2_SPTR as we are working in 8-bit code units */
  pattern        = (PCRE2_SPTR)regex;
  subject        = (PCRE2_SPTR)match;
  subject_length = strlen((char *)subject);

  /* Now we are going to compile the regular expression pattern, and handle any
   * errors that are detected */
  re = pcre2_compile(pattern,               /* the pattern */
                     PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
                     0,                     /* default options */
                     &errornumber,          /* for error number */
                     &erroroffset,          /* for error offset */
                     NULL);                 /* use default compile context */

  /* Compilation failed: print the error message and exit. */
  if (re == NULL) {
    PCRE2_UCHAR buffer[256];
    pcre2_get_error_message(errornumber, buffer, sizeof(buffer));

    return -1;
  }

  /* If the compilation succeeded, we call PCRE again, in order to do a pattern match against the
   * subject string. This does just ONE match. If further matching is needed, it will be done below.
   * Before running the match we must set up a match_data block for holding the result. */

  /* Using this function ensures that the block is exactly the right size for the number of
   * capturing parentheses in the pattern */
  match_data = pcre2_match_data_create_from_pattern(re, NULL);

  rc = pcre2_match(re,             /* the compiled pattern */
                   subject,        /* the subject string */
                   subject_length, /* the length of the subject */
                   0,              /* start at offset 0 in the subject */
                   0,              /* default options */
                   match_data,     /* block for storing the result */
                   NULL);          /* use default match context */

  /* Matching failed: handle error cases */
  if (rc < 0) {
    switch (rc) {
      case PCRE2_ERROR_NOMATCH:
        break;
      default:
        break;
    }

    /* Release memory used for the match data and the compiled pattern. */
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    return -1;
  }

  /* Match succeeded. Get a pointer to the output vector, where string offsets
   * are stored. */
  ovector = pcre2_get_ovector_pointer(match_data);

  /* We have found the first match within the subject string. If the output vector wasn't big
   * enough, say so. Then output any substrings that were captured. */

  /* Check if the output vector wasn't big enough. This should not happen, because we used
   * pcre2_match_data_create_from_pattern() above. */
  if (rc == 0) {
    rc = OVECCOUNT / 3;
  }

  for (int i = 0; i < rc; i++) {
    PCRE2_SPTR substring_start = subject + ovector[2 * i];
    size_t substring_length    = ovector[2 * i + 1] - ovector[2 * i];
  }

  /* The code that follows shows first how to access named substrings */

  /* See if there are any named substrings, and if so, show them by name. First we have to extract
   * the count of named parentheses from the pattern. */
  (void)pcre2_pattern_info(re,                   /* the compiled pattern */
                           PCRE2_INFO_NAMECOUNT, /* get the number of named substrings */
                           &namecount);          /* where to put the answer */

  if (namecount > 0) {
    PCRE2_SPTR tabptr;

    /* Before we can access the substrings, we must extract the table for translating names to
     * numbers, and the size of each entry in the table. */

    (void)pcre2_pattern_info(re,                       /* the compiled pattern */
                             PCRE2_INFO_NAMETABLE,     /* address of the table */
                             &name_table);             /* where to put the answer */

    (void)pcre2_pattern_info(re,                       /* the compiled pattern */
                             PCRE2_INFO_NAMEENTRYSIZE, /* size of each entry in the table */
                             &name_entry_size);        /* where to put the answer */

    /* Now we can scan the table and, for each entry, print the number, the name, and the substring
     * itself. In the 8-bit library the number is held in two bytes, most significant first. */
    tabptr = name_table;
    for (int i = 0; i < namecount; i++) {
      int n = (tabptr[0] << 8) | tabptr[1];
      tabptr += name_entry_size;
    }
  }

  /* Cleanup */
  if (!repeat) {
    pcre2_match_data_free(match_data); /* Release the memory that was used */
    pcre2_code_free(re);               /* for the match data and the pattern. */
    return 0;                          /* Exit the program. */
  }

  (void)pcre2_pattern_info(re, PCRE2_INFO_ALLOPTIONS, &option_bits);
  utf8 = (option_bits & PCRE2_UTF) != 0;

  /* Now find the newline convention and see whether CRLF is a valid newline sequence */
  (void)pcre2_pattern_info(re, PCRE2_INFO_NEWLINE, &newline);
  crlf_is_newline = newline == PCRE2_NEWLINE_ANY || newline == PCRE2_NEWLINE_CRLF ||
                    newline == PCRE2_NEWLINE_ANYCRLF;

  /* Loop for second and subsequent matches */
  for (;;) {
    uint32_t options        = 0;          /* Normally no options */
    PCRE2_SIZE start_offset = ovector[1]; /* Start at end of previous match */

    /* If the previous match was for an empty string, we are finished if we are at the end of the
     * subject. Otherwise, arrange to run another match at the same point to see if a non-empty
     * match can be found. */

    if (ovector[0] == ovector[1]) {
      if (ovector[0] == subject_length) {
        break;
      }
      options = PCRE2_NOTEMPTY_ATSTART | PCRE2_ANCHORED;
    }

    /* Run the next matching operation */
    rc = pcre2_match(re,             /* the compiled pattern */
                     subject,        /* the subject string */
                     subject_length, /* the length of the subject */
                     start_offset,   /* starting offset in the subject */
                     options,        /* options */
                     match_data,     /* block for storing the result */
                     NULL);          /* use default match context */

    /* This time, a result of NOMATCH isn't an error. If the value in [options] is zero, it just
     * means we have found all possible matches, so the loop ends. Otherwise, it means we have
     * failed to find a non-empty-string match at a point where there was a previous empty-string
     * match. In this case, we do what Perl does: advance the matching position by one character,
     * and continue. We do this by setting the "end of previous match" offset, because that is
     * picked up at the top of the loop as the point at which to start again. There are two
     * complications: (a) When CRLF is a valid newline sequence, and the current position is just
     * before it, advance by an extra byte. (b) Otherwise we must ensure hat we skip an entire UTF
     * character if we are in UTF mode. */

    if (rc == PCRE2_ERROR_NOMATCH) {
      if (options == 0) {
        /* All matches found */
        break;
      }
      ovector[1] = start_offset + 1;           /* Advance one code unit */
      if (crlf_is_newline &&                   /* If CRLF is newline & */
          start_offset < subject_length - 1 && /* we are at CRLF, */
          subject[start_offset] == '\r' && subject[start_offset + 1] == '\n') {
        ovector[1] += 1;                       /* Advance by one more. */
      } else if (utf8) {                       /* Else ensure we advance a whole UTF-8 character */
        while (ovector[1] < subject_length) {
          if ((subject[ovector[1]] & 0xc0) != 0x80) {
            break;
          }
          ovector[1] += 1;
        }
      }
      continue; /* Go round the loop again */
    }

    /* Other matching errors are not recoverable */
    if (rc < 0) {
      pcre2_match_data_free(match_data);
      pcre2_code_free(re);
      return -1;
    }

    /* The match succeeded, but the output vector wasn't big enough. This should not happen. */
    if (rc == 0) {
      rc = OVECCOUNT / 3;
    }

    /* As before, show substrings stored in the output vector by number, and then also any named
     * substrings. */
    for (int i = 0; i < rc; i++) {
      PCRE2_SPTR substring_start = subject + ovector[2 * i];
      size_t substring_length    = ovector[2 * i + 1] - ovector[2 * i];
    }

    if (namecount > 0) {
      PCRE2_SPTR tabptr = name_table;
      for (int j = 0; j < namecount; j++) {
        int n = (tabptr[0] << 8) | tabptr[1];
        tabptr += name_entry_size;
      }
    }
  } /* End of loop to find second and subsequent matches */

  /* Cleanup */
  pcre2_match_data_free(match_data);
  pcre2_code_free(re);

  return 0;
}

#endif  // S_PCRE

#if !defined(S_PCRE) && !defined(S_PCRE2)

static bool internal_posix_match(const char *pattern, const char *str) {
  regex_t regex;
  int rc = regcomp(&regex, pattern, REG_EXTENDED);
  if (rc != 0) {
    return false;
  }

  rc = regexec(&regex, str, 0, NULL, 0);
  regfree(&regex);

  return rc == 0;
}

static char *internal_posix_find(const char *pattern, const char *str) {
  regex_t regex;
  regmatch_t match;
  int rc = regcomp(&regex, pattern, REG_EXTENDED);
  if (rc != 0) {
    return NULL;
  }

  rc = regexec(&regex, str, 1, &match, 0);
  if (rc != 0) {
    regfree(&regex);
    return NULL;
  }

  size_t len = (size_t)(match.rm_eo - match.rm_so);
  char *result = (char *)malloc(len + 1);
  if (result == NULL) {
    regfree(&regex);
    return NULL;
  }

  memcpy(result, str + match.rm_so, len);
  result[len] = S_NULL_CHAR;

  regfree(&regex);
  return result;
}

static char *internal_posix_between(const char *str,
                                    const char *leftPattern,
                                    const char *rightPattern) {
  regex_t left;
  regex_t right;
  regmatch_t left_match;
  regmatch_t right_match;

  if (regcomp(&left, leftPattern, REG_EXTENDED) != 0) {
    return NULL;
  }

  if (regexec(&left, str, 1, &left_match, 0) != 0) {
    regfree(&left);
    return NULL;
  }

  const char *after_left = str + left_match.rm_eo;

  if (regcomp(&right, rightPattern, REG_EXTENDED) != 0) {
    regfree(&left);
    return NULL;
  }

  if (regexec(&right, after_left, 1, &right_match, 0) != 0) {
    regfree(&right);
    regfree(&left);
    return NULL;
  }

  size_t start = (size_t)left_match.rm_eo;
  size_t end = start + (size_t)right_match.rm_so;
  size_t len = end - start;

  char *result = (char *)malloc(len + 1);
  if (result == NULL) {
    regfree(&right);
    regfree(&left);
    return NULL;
  }

  memcpy(result, str + start, len);
  result[len] = S_NULL_CHAR;

  regfree(&right);
  regfree(&left);
  return result;
}

#endif  // !S_PCRE && !S_PCRE2

bool regex_match(const char *pattern, const char *str) {
#ifdef S_PCRE
  return internal_pcre_match(pattern, str, false) == 0;
#elif S_PCRE2
  return internal_pcre2_match(pattern, str, false) == 0;
#else
  return internal_posix_match(pattern, str);
#endif  // S_PCRE
}

char *regex_find(const char *pattern, const char *str) {
#ifdef S_PCRE
  return internal_prce_find(pattern, str);
#elif S_PCRE2
  return internal_prce_find(pattern, str);
#else
  return internal_posix_find(pattern, str);
#endif
}

char *regex_between(const char *str,
                                        const char *leftPattern,
                                        const char *rightPattern) {
#ifdef S_PCRE
  return internal_prce_between(str, leftPattern, rightPattern);
#elif S_PCRE2
  return internal_prce_between(str, leftPattern, rightPattern);
#else
  return internal_posix_between(str, leftPattern, rightPattern);
#endif
}
