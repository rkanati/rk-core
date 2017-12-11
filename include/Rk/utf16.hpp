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
  char16* utf16_encode (char32 cp, char16* dest, char16* limit);

  class UTF16Encoder {
  public:
    using DestUnit = char16;
    static constexpr size_t min_buffer = 2;

    DestUnit* operator () (char32 cp, DestUnit* dest, DestUnit* limit) const {
      return utf16_encode (cp, dest, limit);
    }
  };

  //
  // UTF16Decoder
  // State machine for decoding UTF-16 streams.
  // Tolerant of invalid input, but warns about it pedantically.
  //
  class UTF16Decoder {
    char16 const* src;
    char16 const* end;
    char32        cp;
    bool          midway;

    bool   empty () const;
    char16 peek () const;
    void   consume ();

  public:
    using SrcUnit = char16;

    UTF16Decoder () :
      src    (nullptr),
      end    (nullptr),
      cp     (0),
      midway (false)
    { }

    UTF16Decoder (char16 const* new_src, char16 const* new_end) :
      UTF16Decoder ()
    {
      set_source (new_src, new_end);
    }

    void set_source (char16 const* new_src, char16 const* new_end);

    char32 codepoint () const {
      return cp;
    }

    using Status = DecodeStatus;

    Status decode ();
  };
}

