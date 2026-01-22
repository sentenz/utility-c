# `/utility-c`

- authentication/password.h
  > An abstraction layer for password hashing and verification. It includes functions for hashing a password using a secure algorithm and verifying a password against a hashed password.

- bit.h
  >  An abstraction layer for common bit manipulation functions. It includes functions for setting, clearing, and checking the value of a bit in a bit array.

- char.h
  > An abstraction layer for common string manipulation functions. It includes functions for converting a string to lowercase, counting the number of occurrences of a substring in a string, replacing a substring with another substring in a string, checking if a string contains a substring, and freeing the memory allocated for a string.

- config.h
  > A set of standard and specific definitions. It includes definitions for ignoring unused parameters, defining the NULL pointer, and defining the maximum length of a file name. The file also includes specific definitions for the NULL character, Unix, Linux, POSIX, file system, and regex library PCRE. Additionally, the file includes macro checks for the include module of regex, the regex module, and the file system module.

- fs.h
  > An abstraction layer for file system operations. It includes functions for creating and removing directories, checking if a file exists in a directory, and getting the size of a file.

- mutex.h
  > An abstraction layer for mutex implementation. It includes functions for locking and unlocking a mutex, and returns 0 on success and -1 on failure. The file defines a s_mutex_t type that is implemented using either the POSIX pthread library or the Cygwin kernel API, depending on the platform.

- regex.h
  > An abstraction layer for working with regular expressions. It includes functions for matching a regular expression pattern against a string and finding the first occurrence of a regular expression pattern in a string. The file uses the PCRE library for regular expression matching.

- types.h
  > An abstraction layer for common types used in C programming. It includes definitions for a mutex type that is implemented using either the POSIX pthread library or the Cygwin kernel API, depending on the platform.

- uri.h
  > An abstraction layer for extracting components of a Uniform Resource Identifier (URI) string. It includes functions for extracting the scheme, host, port, and user components of a URI string, and returns each component as a dynamically allocated string that the caller is responsible for freeing.
  >
  > NOTE Based on the URI parser library [uriparser](https://github.com/uriparser/uriparser/tree/uriparser-0.9.7).

- util.h
  > An abstraction layer for common utility functions. It includes functions for checking if a number is a floating point number, counting the number of digits in an integer or floating point number, and checking if two floating point numbers are approximately equal.

- xml.h
  > An abstraction layer for working with XML data. It includes functions for converting an XML string to an XML tree, creating an empty XML file, and freeing the memory used by an XML tree.
  >
  > NOTE Based on the Mini-XML library for parsing and manipulating XML data [mxml](https://github.com/michaelrsweet/mxml).
