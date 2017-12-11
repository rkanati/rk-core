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

namespace Rk {
  namespace detail {
    template <typename Decoder, typename Encoder>
    class UTFTranscoderImpl {
    public:
      typedef typename Decoder::SrcUnit  SrcUnit;
      typedef typename Encoder::DestUnit DestUnit;

    private:
      Decoder  dec;
      Encoder  enc;
      DestUnit buf [Encoder::min_buffer];
      size_t   n;

    public:
      UTFTranscoderImpl (const SrcUnit* src, const SrcUnit* end) :
        dec (src, end),
        n   (0)
      { }

      void set_source (const SrcUnit* new_src, const SrcUnit* new_end) {
        dec.set_source (new_src, new_end);
      }

      using Status = DecodeStatus;

      Status decode () {
        auto stat = dec.decode ();

        if (stat == Status::idle || stat == Status::pending)
          return stat;

        auto ptr = enc (dec.codepoint (), std::begin (buf), std::end (buf));
        n = ptr - buf;
        return stat;
      }

      DestUnit const* units () const {
        return buf;
      }

      DestUnit const* units_end () const {
        return buf + n;
      }

      size_t unit_count () const {
        return n;
      }
    };
  }

  using UTF8To16 = detail::UTFTranscoderImpl <UTF8Decoder, UTF16Encoder>;
  using UTF16To8 = detail::UTFTranscoderImpl <UTF16Decoder, UTF8Encoder>;

  namespace detail {
    template <typename Coder>
    static auto string_utf_convert_impl (StringRefBase <typename Coder::SrcUnit> src, bool tolerant)
      -> std::basic_string <typename Coder::DestUnit>
    {
      std::basic_string <typename Coder::DestUnit> result;
      Coder coder (src.begin (), src.end ());

      for (;;) {
        auto stat = coder.decode ();
        if (stat == DecodeStatus::idle || (tolerant && stat == DecodeStatus::pending))
          break;
        else if (stat != DecodeStatus::got_codepoint && !tolerant)
          throw std::runtime_error ("string_utf_convert_impl - decode error");
        result.append (coder.units (), coder.unit_count ());
      }

      return result;
    }
  }

  static inline std::u16string string_utf8_to_16 (StringRef src, bool tolerant = false) {
    return detail::string_utf_convert_impl <UTF8To16> (src, tolerant);
  }

  static inline std::string string_utf16_to_8 (U16StringRef src, bool tolerant = false) {
    return detail::string_utf_convert_impl <UTF16To8> (src, tolerant);
  }
}

