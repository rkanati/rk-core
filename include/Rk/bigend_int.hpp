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

namespace Rk {
  namespace bigend_private {
    extern "C" {
      unsigned __int64 __cdecl _byteswap_uint64 (unsigned __int64);
      #pragma intrinsic (_byteswap_uint64)
      ulong __cdecl _byteswap_ulong (ulong);
      #pragma intrinsic (_byteswap_ulong)
      ushort __cdecl _byteswap_ushort (ushort);
      #pragma intrinsic (_byteswap_ushort)
    }
  }

  static inline u64 byteswap (u64 value) { return static_cast <u64> (bigend_private::_byteswap_uint64 (static_cast <unsigned __int64> (value))); }
  static inline u32 byteswap (u32 value) { return static_cast <u32> (bigend_private::_byteswap_ulong  (static_cast <ulong>            (value))); }
  static inline u16 byteswap (u16 value) { return static_cast <u16> (bigend_private::_byteswap_ushort (static_cast <ushort>           (value))); }

  template <typename under>
  class bigend_int {
    under val;

  public:
    bigend_int () = default;

    explicit bigend_int (under v) :
      val (byteswap (v))
    { }

    under get () const {
      return byteswap (val);
    }

    template <typename out_stream>
    friend out_stream& operator << (out_stream& stream, const bigend_int& value) {
      return stream << value.get ();
    }
  };

  namespace types {
    typedef bigend_int <u16> u16be;
    typedef bigend_int <u32> u32be;
    typedef bigend_int <u64> u64be;
  }
}

