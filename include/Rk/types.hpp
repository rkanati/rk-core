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

#include <cstddef>
#include <cstdint>

namespace Rk {
  namespace types {
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

    typedef wchar_t  wchar;
    typedef char16_t char16;
    typedef char32_t char32;

    typedef intptr_t  iptr;
    typedef uintptr_t uptr;

    typedef float  f32; static_assert (sizeof (f32) == 4, "float not 32 bit" );
    typedef double f64; static_assert (sizeof (f64) == 8, "double not 64 bit");

    template <uint bits> struct integer_of_size;
    template <> struct integer_of_size < 8> { typedef u8  unsigned_t; typedef i8  signed_t; };
    template <> struct integer_of_size <16> { typedef u16 unsigned_t; typedef i16 signed_t; };
    template <> struct integer_of_size <32> { typedef u32 unsigned_t; typedef i32 signed_t; };
    template <> struct integer_of_size <64> { typedef u64 unsigned_t; typedef i64 signed_t; };

    template <uint bits>
    using integer_of_size_t = typename integer_of_size <bits>::signed_t;

    template <uint bits>
    using signed_of_size_t = typename integer_of_size <bits>::signed_t;

    template <uint bits>
    using unsigned_of_size_t = typename integer_of_size <bits>::unsigned_t;

    template <typename T, uint bits = sizeof (T) * 8>
    using integer_same_size = integer_of_size <bits>;

    template <typename T>
    using integer_same_size_t = typename integer_same_size <T>::signed_t;

    template <typename T>
    using signed_same_size_t = typename integer_same_size <T>::signed_t;

    template <typename T>
    using unsigned_same_size_t = typename integer_same_size <T>::unsigned_t;

    struct nil_t      { } static constexpr nil = { };
    struct identity_t { } static constexpr identity = { };
  }

  using namespace types;
}

#ifndef RK_TYPES_NO_GLOBAL
using namespace Rk::types;
#endif

