//
// Copyright (C) 2013 Rk
// Permission to use, copy, modify and distribute this file for any purpose is hereby granted without fee or other
// limitation, provided that
//  - this notice is reproduced verbatim in all copies and modifications of this file, and
//  - neither the names nor trademarks of the copyright holder are used to endorse any work which includes or is derived
//    from this file.
// No warranty applies to the use, either of this software, or any modification of it, and Rk disclaims all liabilities
// in relation to such use.
//

// Tests
#include <Rk/utf8.hpp>

// Uses
#include <Rk/string_ref.hpp>

#include "test.hpp"

namespace RkTest {
  bool test_utf8_encode (u32 cp, Rk::cstring_ref canon) {
    char buf [4];
    auto end = Rk::utf8_encode (cp, buf, buf + 4);
    Rk::cstring_ref spelling (buf, end);
    return spelling == canon;
  }

  bool test_utf8_encoding () try {
    bool ok = true;

    char nul = 0;
    TEST (test_utf8_encode (u32 ('\0'),     Rk::cstring_ref (&nul, &nul + 1)));
    TEST (test_utf8_encode (u32 ('A'),      "A"));
    TEST (test_utf8_encode (u32 (0x00007f), "\x7f"));
    TEST (test_utf8_encode (u32 (0x0003a6), "\xce\xa6")); // U+3A6 GREEK CAPITAL LETTER PHI
    TEST (test_utf8_encode (u32 (0x00203d), "\xe2\x80\xbd")); // U+203D INTERROBANG
    TEST (test_utf8_encode (u32 (0x10fffd), "\xf4\x8f\xbf\xbd")); // U+10fffd PRIVATE USE

    return ok;
  }
  catch (...) {
    return false;
  }
}

