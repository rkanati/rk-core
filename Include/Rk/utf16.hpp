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

#ifndef RK_CORE_API
#define RK_CORE_API __declspec(dllimport)
#endif

namespace Rk
{
  RK_CORE_API char16* utf16_encode (char32 cp, char16* dest, char16* limit);

  class utf16_encoder
  {
  public:
    using dest_t = char16;

    dest_t* operator () (char32 cp, dest_t* dest, dest_t* limit) const
    {
      return utf16_encode (cp, dest, limit);
    }

  };

  //
  // utf16_decoder
  // State machine for decoding UTF-16 streams.
  // Tolerant of invalid input, but warns about it pedantically.
  //
  class utf16_decoder
  {
    const char16* src;
    const char16* end;
    char32        cp;
    bool          midway;

    RK_CORE_API bool   empty () const;
    RK_CORE_API char16 peek () const;
    RK_CORE_API void   consume ();

  public:
    utf16_decoder () :
      src    (nullptr),
      end    (nullptr),
      cp     (0),
      midway (false)
    { }

    utf16_decoder (const char16* new_src, const char16* new_end) :
      utf16_decoder ()
    {
      set_source (new_src, new_end);
    }

    RK_CORE_API void set_source (const char16* new_src, const char16* new_end);

    char32 codepoint () const
    {
      return cp;
    }

    using status_t = decode_status_t;

    RK_CORE_API status_t decode ();

  };

}
