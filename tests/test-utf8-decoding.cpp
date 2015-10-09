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
  using status_t = Rk::utf8_decoder::status_t;

  bool test_utf8_decode (Rk::cstring_ref src, status_t expect_stat, u32 cp) {
    Rk::utf8_decoder dec (src.begin (), src.end ());
    auto stat = dec.decode ();
    return (stat == expect_stat) && (dec.codepoint () == cp);
  }

  bool test_utf8_decode_valid () {
    bool ok = true;
    TEST (test_utf8_decode ("A", status_t::got_codepoint, 0x41));
    TEST (test_utf8_decode ("\xce\xa9", status_t::got_codepoint, 0x3a9)); // U+03A9 GREEK LETTER CAPITAL OMEGA
    TEST (test_utf8_decode ("\xe2\x99\xa0", status_t::got_codepoint, 0x2660)); // U+2660 BLACK SPADE SUIT
    TEST (test_utf8_decode ("\xf0\xa1\x83\xbe", status_t::got_codepoint, 0x210fe)); // U+210FE CJK IDEOGRAPH
    return ok;
  }

  bool test_utf8_decode_overlong () {
    bool ok = true;
    TEST (test_utf8_decode ("\xc1\x81", status_t::got_overlong, 0x41));
    TEST (test_utf8_decode ("\xe0\x81\x81", status_t::got_overlong, 0x41));
    TEST (test_utf8_decode ("\xf0\x80\x81\x81", status_t::got_overlong, 0x41));
    return ok;
  }

  bool test_utf8_decode_surrogate () {
    bool ok = true;
    TEST (test_utf8_decode ("\xed\xa0\x80", status_t::got_surrogate, 0xd800));
    return ok;
  }

  bool test_utf8_decode_nonchar () {
    bool ok = true;
    TEST (test_utf8_decode ("\xef\xb7\x90", status_t::got_noncharacter, 0xfdd0));
    TEST (test_utf8_decode ("\xef\xbf\xbe", status_t::got_noncharacter, 0xfffe));
    TEST (test_utf8_decode ("\xf2\x8f\xbf\xbf", status_t::got_noncharacter, 0x8ffff));
    return ok;
  }

  bool test_utf8_decode_bad () {
    bool ok = true;
    TEST (test_utf8_decode ("\xf7\xbf\xbf\xbf", status_t::bad_codepoint, 0x1fffff));
    return ok;
  }

  bool test_utf8_decode_invalid () {
    bool ok = true;
    TEST (test_utf8_decode ("\xc0\x01", status_t::invalid_sequence, 0xfffd)); // truncated 2-seq
    TEST (test_utf8_decode ("\x80", status_t::invalid_sequence, 0xfffd)); // orphaned continuation
    return ok;
  }

  bool test_utf8_decoding () try {
    bool ok = true;

    TEST (test_utf8_decode_valid ());
    TEST (test_utf8_decode_overlong ());
    TEST (test_utf8_decode_surrogate ());
    TEST (test_utf8_decode_nonchar ());
    TEST (test_utf8_decode_bad ());
    TEST (test_utf8_decode_invalid ());

    return ok;
  }
  catch (...) {
    return false;
  }
}

