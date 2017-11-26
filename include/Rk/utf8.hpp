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

#pragma once

#include <Rk/types.hpp>

#include <Rk/unicode_common.hpp>

namespace Rk {
  int utf8_code_length (char32 cp);

  char* utf8_encode (char32 cp, char* dest, char* limit);

  class UTF8Encoder {
  public:
    using DestUnit = char;
    static constexpr size_t min_buffer = 4;

    DestUnit* operator () (char32 cp, DestUnit* dest, DestUnit* limit) const {
      return utf8_encode (cp, dest, limit);
    }
  };

  //
  // utf8_decoder
  // State machine for decoding UTF-8 streams.
  // Tolerant of invalid input, but warns about it pedantically.
  //
  class UTF8Decoder {
    char const* src;
    char const* end;
    char32      cp;
    int         len,
                pos;

    bool  empty () const;
    uchar peek () const;
    void  consume ();

  public:
    using SrcUnit = char;

    UTF8Decoder () :
      src (nullptr),
      end (nullptr),
      cp  (0),
      len (0),
      pos (0)
    { }

    UTF8Decoder (char const* new_src, char const* new_end):
      UTF8Decoder ()
    {
      set_source (new_src, new_end);
    }

    void set_source (char const* new_src, char const* new_end);

    char const* get_pointer () const {
      return src;
    }

    char32 codepoint () const {
      return cp;
    }

    using Status = DecodeStatus;

    Status decode ();
  };
}

