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

#include <cstdint>

namespace Rk
{
  namespace Types
  {
    typedef int8_t   i8;
    typedef uint8_t  u8;
    typedef int16_t  i16;
    typedef uint16_t u16;
    typedef int32_t  i32;
    typedef uint32_t u32;
    typedef int64_t  i64;
    typedef uint64_t u64;

    typedef unsigned char  uchar;
    typedef unsigned short ushort;
    typedef unsigned int   uint;
    typedef unsigned long  ulong;

    typedef char16_t char16;
    typedef char32_t char32;

    typedef intptr_t  iptr;
    typedef uintptr_t uptr;

    typedef float  f32; static_assert (sizeof (f32) == 4, "float not 32 bit" );
    typedef double f64; static_assert (sizeof (f64) == 8, "double not 64 bit");

    template <uint bits> struct IntegerOfSize;
    template <> struct IntegerOfSize < 8> { typedef u8  Unsigned; typedef i8  Signed; };
    template <> struct IntegerOfSize <16> { typedef u16 Unsigned; typedef i16 Signed; };
    template <> struct IntegerOfSize <32> { typedef u32 Unsigned; typedef i32 Signed; };
    template <> struct IntegerOfSize <64> { typedef u64 Unsigned; typedef i64 Signed; };

    class Nil
    {
      void operator& () const;
      
    };

    static Nil nil = { };

    template <typename OStream>
    OStream& operator << (OStream& os, const Nil&)
    {
      return os;
    }

  }
  
  using namespace Rk::Types;

}

#ifndef RK_TYPES_NO_GLOBAL
using namespace Rk::Types;
#endif