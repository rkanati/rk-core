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

#include <Rk/string_ref.hpp>
#include <Rk/types.hpp>

#include <Rk/utf16.hpp>
#include <Rk/utf8.hpp>

#include <string>

namespace Rk
{
  class utf8_to_16
  {
    utf8_decoder dec;
    char16       buf [2];
    size_t       n;

  public:
    utf8_to_16 (const char* src, const char* end) :
      dec (src, end),
      n (0)
    { }

    void set_source (const char* new_src, const char* new_end)
    {
      dec.set_source (new_src, new_end);
    }

    using status_t = decode_status_t;

    status_t decode ()
    {
      auto stat = dec.decode ();

      if (stat == idle || stat == pending)
        return stat;

      auto ptr = utf16_encode (dec.codepoint (), buf, buf + 2);
      n = ptr - buf;
      return stat;
    }

    const char16* units () const
    {
      return buf;
    }

    const char16* units_end () const
    {
      return buf + n;
    }

    size_t unit_count () const
    {
      return n;
    }

  };

  static inline std::u16string string_utf8_to_u16 (cstring_ref src, bool tolerant = false)
  {
    std::u16string result;
    utf8_to_16 coder (src.begin (), src.end ());

    for (;;)
    {
      auto stat = coder.decode ();
      if (stat == idle || tolerant && stat == pending)
        break;
      else if (stat != got_codepoint && !tolerant)
        throw std::runtime_error ("string_utf8_to_u16 - decode error");
      result.append (coder.units (), coder.unit_count ());
    }

    return result;
  }

}
