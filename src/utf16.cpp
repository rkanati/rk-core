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

#include <Rk/utf16.hpp>

#include <stdexcept>

namespace Rk {
  char16* utf16_encode (char32 cp, char16* dest, char16* limit) {
    // Not enough space?
    if (limit - dest < 2)
      return dest;

    // Invalid codepoint?
    if (!is_codepoint_ordinary (cp))
      return dest;

    // Encode
    if (cp <= 0xfff) {
      *dest++ = char16 (cp);
    }
    else {
      cp -= 0x10000;
      *dest++ = 0xd800 | char16 (cp >> 10);
      *dest++ = 0xdc00 | char16 (cp & 0x3ff);
    }

    return dest;
  }

  bool UTF16Decoder::empty () const {
    return src == end;
  }

  char16 UTF16Decoder::peek () const {
    return *src;
  }

  void UTF16Decoder::consume () {
    src++;
  }

  void UTF16Decoder::set_source (const char16* new_src, const char16* new_end) {
    if (!new_src || new_end < new_src)
      throw std::length_error ("Invalid source range");

    src = new_src;
    end = new_end;
  }

  auto UTF16Decoder::decode ()
    -> Status
  {
    // Decode single or lead surrogate
    if (!midway) {
      if (empty ()) return Status::idle;
      char16 word = peek ();
      consume ();

      // Single?
      if (word < 0xd800 || word > 0xdfff)
        cp = word;
      else if (word >= 0xdc00) { // Trailing surrogates not valid here
        midway = false;
        cp = 0xfffd;
        return Status::invalid_sequence;
      }
      else {
        cp = (word & 0x3ff) << 10;
        midway = true;
      }
    }

    // Decode trail surrogate
    if (midway) {
      if (empty ()) return Status::pending;
      char16 word = peek ();

      // Valid trail surrogate?
      if (word < 0xdc00 || word > 0xdfff) {
        midway = false;
        cp = 0xfffd;
        return Status::invalid_sequence;
      }

      cp = 0x10000 + (cp | (word & 0x3ff));
      consume ();
    }

    // Diagnose dodgy sequences
    auto stat = Status::got_codepoint;

    if (is_codepoint_noncharacter (cp))
      stat = Status::got_noncharacter;

    midway = false; // Expect a new codepoint
    return stat;
  }
}

