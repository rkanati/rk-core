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
  namespace detail
  {
    template <typename decoder_t, typename encoder_t>
    class utf_transcoder_impl
    {
    public:
      typedef typename decoder_t::src_t  src_t;
      typedef typename encoder_t::dest_t dest_t;

    private:
      decoder_t dec;
      encoder_t enc;
      dest_t    buf [encoder_t::min_buffer];
      size_t    n;

    public:
      utf_transcoder_impl (const src_t* src, const src_t* end) :
        dec (src, end),
        n   (0)
      { }

      void set_source (const src_t* new_src, const src_t* new_end)
      {
        dec.set_source (new_src, new_end);
      }

      using status_t = decode_status_t;

      status_t decode ()
      {
        auto stat = dec.decode ();

        if (stat == idle || stat == pending)
          return stat;

        auto ptr = enc (dec.codepoint (), std::begin (buf), std::end (buf));
        n = ptr - buf;
        return stat;
      }

      const dest_t* units () const
      {
        return buf;
      }

      const dest_t* units_end () const
      {
        return buf + n;
      }

      size_t unit_count () const
      {
        return n;
      }

    };

  } // detail

  using utf8_to_16 = detail::utf_transcoder_impl <utf8_decoder, utf16_encoder>;
  using utf16_to_8 = detail::utf_transcoder_impl <utf16_decoder, utf8_encoder>;

  namespace detail
  {
    template <typename decoder_t, typename dest_unit_t, typename src_unit_t>
    static auto string_utf_convert_impl (string_ref_base <src_unit_t> src, bool tolerant)
      -> std::basic_string <dest_unit_t>
    {
      std::basic_string <dest_unit_t> result;
      decoder_t coder (src.begin (), src.end ());

      for (;;)
      {
        auto stat = coder.decode ();
        if (stat == idle || tolerant && stat == pending)
          break;
        else if (stat != got_codepoint && !tolerant)
          throw std::runtime_error ("string_utf_convert_impl - decode error");
        result.append (coder.units (), coder.unit_count ());
      }

      return result;
    }

  }

  static inline std::u16string string_utf8_to_16 (cstring_ref src, bool tolerant = false)
  {
    return detail::string_utf_convert_impl <utf8_to_16, char16> (src, tolerant);
  }

  static inline std::string string_utf16_to_8 (u16string_ref src, bool tolerant = false)
  {
    return detail::string_utf_convert_impl <utf16_to_8, char> (src, tolerant);
  }

}
