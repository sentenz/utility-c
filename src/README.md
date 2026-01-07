# `/src`

The purpose and content of `src` depends on whether the project authors choose to follow [§ 3.1.2 Merged Header Placement](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#src.header-placement.merged) or [§ 3.1.1 Separate Header Placement](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#src.header-placement.separate).

See [§ 3 Library Source Layout](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs#src).

- authentication/password.c
  > An abstraction layer for password hashing and verification.

- crypto/argon2.c/.h
  > An abstraction layer for the Argon2 password hashing function.

- bit.c
  > An internal library for [bitwise](https://github.com/keon/awesome-bits) operations.

- char.c
  > An internal library for character oprations.

- fs.c
  > An internal library for file system oprations.

- mutex.c
  > An internal library for mutual exclusion oprations.

- network.c
  > An internal library for network oprations.

- regex.c
  > An abstraction layer for regular expression.
  >
  > NOTE regex is a reimplementation of:
  >
  > - the [pcredemo.c](https://github.com/vmg/pcre/blob/master/pcredemo.c) example for the [PCRE (Perl-Compatible Regular Expression)](https://github.com/vmg/pcre) v8.44 library.
  > - the [pcre2demo.c](https://github.com/luvit/pcre2/blob/master/src/pcre2demo.c) example for the [PCRE2 (Perl-Compatible Regular Expression)](https://github.com/PCRE2Project/pcre2) v10.36 library.

- uri.c
  > An abstraction layer for the URI parser.
  >
  > NOTE Based on the URI parser library [uriparser](https://github.com/uriparser/uriparser/tree/uriparser-0.9.7).

- util.c
  > An internal library for utility oprations.

- xml.c
  > An abstraction layer for XML file handling.
  >
  > NOTE Based on the XML parsing library [mxml](https://github.com/michaelrsweet/mxml).
