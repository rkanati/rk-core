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
#define RK_CORE_API
#endif

namespace Rk
{
  RK_CORE_API int utf8_code_length (char32 cp);

  RK_CORE_API char* utf8_encode (char32 cp, char* dest, char* limit);

  //
  // utf8_decoder
  // State machine for decoding UTF-8 streams.
  // Tolerant of invalid input, but warns about it pedantically.
  //
  class utf8_decoder
  {
    const char* src;
    const char* end;
    char32      cp;
    int         len,
                pos;

    RK_CORE_API bool  empty () const;
    RK_CORE_API uchar peek () const;
    RK_CORE_API void  consume ();

  public:
    utf8_decoder () :
      src (nullptr),
      end (nullptr),
      cp  (0),
      len (0),
      pos (0)
    { }

    utf8_decoder (const char* new_src, const char* new_end):
      utf8_decoder ()
    {
      set_source (new_src, new_end);
    }

    RK_CORE_API void set_source (const char* new_src, const char* new_end);

    char32 codepoint () const
    {
      return cp;
    }

    using status_t = decode_status_t;

    RK_CORE_API status_t decode ();

  };

}
