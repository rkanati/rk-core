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

#include <Rk/utf8.hpp>

#include <stdexcept>

namespace Rk
{
  int utf8_code_length (char32 cp)
  {
    if      (cp <       0x80) return 1; // 7-bit
    else if (cp <      0x800) return 2; // 11-bit
    else if (cp <    0x10000) return 3; // 16-bit
    else if (cp <   0x200000) return 4; // 21-bit
    else if (cp <  0x4000000) return 5; // 26-bit
    else if (cp < 0x80000000) return 6; // 31-bit
    else return 0;
  }

  char* utf8_encode (char32 cp, char* dest, char* limit)
  {
    // Not enough space?
    if (limit - dest < 4)
      return dest;

    // Invalid codepoint?
    if (!is_codepoint_ordinary (cp))
      return dest;

    int code_len = utf8_code_length (cp);

    // Write first byte
    if (code_len == 1) // 7-bit codepoint
    {
      *dest++ = char (cp & 0x7f);
      return dest;
    }

    // 11, 16, and 21-bit codepoint
    int lead_bits = 7 - code_len,
        cont_bits = 6 * (code_len - 1);

    auto byte = (0xfeu << lead_bits) | (cp >> cont_bits);
    *dest++ = char (byte);

    // Encode continuation bytes
    while (cont_bits != 0)
    {
      cont_bits -= 6;
      auto bits = (cp >> cont_bits) & 0x3f;
      *dest++ = char (0x80 | bits);
    }

    return dest;
  }

  bool utf8_decoder::empty () const
  {
    return src == end;
  }

  uchar utf8_decoder::peek () const
  {
    return uchar (*src);
  }

  void utf8_decoder::consume ()
  {
    src++;
  }

  void utf8_decoder::set_source (const char* new_src, const char* new_end)
  {
    if (!new_src || new_end < new_src)
      throw std::length_error ("Invalid source range");

    src = new_src;
    end = new_end;
  }

  auto utf8_decoder::decode ()
    -> status_t
  {
    if (empty ()) return need_data;
    uchar byte = peek ();

    // Expecting fresh sequence?
    if (len == 0)
    {
      // We want to drop invalid bytes here anyway
      consume ();

      // 7-bit code byte
      if ((byte & 0x80) == 0x00)
      {
        cp = byte;
        return got_codepoint;
      }

      // Possible lead byte; try different prefixes/sequence lengths
      uchar mask = 0xe0;
      for (len = 2; len <= 6; len++)
      {
        if ((byte & mask) == uchar (mask << 1))
          break;
        mask = 0x80 | (mask >> 1);
      }

      // Not a lead byte
      if (len > 6)
        goto fail;

      // Grab the first few bits
      cp = byte & ~mask;
      pos = 1;
    }

    // Decode continuations
    while (pos++ != len)
    {
      if (empty ()) return need_data;
      byte = peek ();

      // Not a continuation
      if ((byte & 0xc0) != 0x80)
        goto fail;

      // Ok; grab bits
      cp = (cp << 6) | (byte & 0x3f);
      consume ();
    }

    // Diagnose dodgy sequences
    status_t stat = got_codepoint;

    auto sem = codepoint_semantic (cp);
    if (sem == codepoint_bad)
      stat = bad_codepoint;
    else if (sem == codepoint_lead_surrogate || sem == codepoint_trail_surrogate)
      stat = got_surrogate;
    else if (sem == codepoint_noncharacter)
      stat = got_noncharacter;
    else if (utf8_code_length (cp) < len)
      stat = got_overlong;

    len = 0; // Expect a new codepoint
    return stat;

    fail:
    len = 0; // re-sync
    cp = 0xfffd; // U+FFFD REPLACEMENT CHARACTER
    return invalid_sequence;
  }

}
