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

#include <Rk/TypeTraits.hpp>
#include <Rk/Types.hpp>

#include <algorithm>
#include <memory>

namespace Rk
{
  // Sets length bytes starting at p to value
  static inline void* memory_set (void* p, uptr length, u8 value)
  {
#ifndef RK_MEMORY_NO_STD
    std::memset (p, value, length);
    return (u8*) p + length;
#else
    auto d = (u8*) p;
    auto e = d + length;
    while (d != e)
      *d++ = value;
    return e;
#endif
  }
  
  // Copies length bytes starting at source to those starting at dest
  template <typename T>
  static inline T* memory_copy (T* dest, const void* source, uptr length)
  {
#ifndef RK_MEMORY_NO_STD
    std::memcpy (dest, source, length);
    return (T*) ((u8*) dest + length);
#else
    auto d = (u8*) dest;
    auto s = (const u8*) source;
    auto e = d + length;
    while (d != e)
      *d++ = *s++;
    return (T*) e;
#endif
  }

  // Fills length bytes starting at p with repeating/truncated raw copies of value
  static inline void* memory_fill (void* const dest_raw, uptr dest_length, const void* src, uptr src_length)
  {
    auto dest = (u8*) dest_raw,
         end  = dest + dest_length;

    while (dest != end)
      dest = memory_copy (dest, src, std::min (end - dest, iptr (src_length)));

    return end;
  }

  template <typename T>
  static inline void* memory_fill (void* const dest, uptr length, const T& value)
  {
    return memory_fill (dest, length, &value, sizeof (value));
  }

  // Returns true iff (u8*) a [i] == (u8*) b [i] for all 0 <= i < length
  static inline bool memory_equal (const void* a, const void* b, uptr length)
  {
#ifndef RK_MEMORY_NO_STD
    return std::memcmp (a, b, length) == 0;
#else
    auto p = (const u8*) a,
         q = (const u8*) b,
         e = p + length;
    while (p != e)
    {
      if (*p++ != *q++)
        return false;
    }
    return true;
#endif
  }
  
  // Returns true if (u8*) a [i] == (u8*) &v [i] for all 0 <= i < sizeof (v)
  template <typename T>
  static inline bool memory_equal (const void* a, const T& v)
  {
    return memory_equal (a, &v, sizeof (T));
  }
  
  // Returns the least multiple of alignment greater or equal to value
  template <typename T, typename U>
  static inline T align (T value, U alignment)
  {
    const auto mod = value % alignment;
    return mod ? value + (alignment - mod) : value;
    //return (value + alignment - 1) / alignment;
  }
  
  template <typename T, typename U>
  static inline T* align (T* p, U alignment)
  {
    return reinterpret_cast <T*> (align (reinterpret_cast <uptr> (p), alignment));
  }

  // Faster alternative to align for power-of-two alignments
  template <typename T, typename U>
  static inline T align2 (T value, U alignment)
  {
    return (value + alignment - 1) & (alignment - 1);
  }

  template <typename T, typename U>
  static inline T* align2 (T* p, U alignment)
  {
    return reinterpret_cast <T*> (align2 (reinterpret_cast <uptr> (p), alignment));
  }

  // Returns the index'th T in the array starting at ptr where each consecutive pair of elements
  // start at addresses stride bytes apart
  template <typename T>
  static inline T& sparse_get (T* ptr, uptr stride, uptr index)
  {
    return *(T*) ((u8*) ptr + stride * index);
  }
  
  template <typename T>
  static inline const T& sparse_get (const T* ptr, uptr stride, uptr index)
  {
    return *(const T*) ((const u8*) ptr + stride * index);
  }
  
  // Returns the length of arbitrary null-terminated strings
  template <typename Char>
  static inline uptr string_length (const Char* s)
  {
    uptr len = 0;
    while (s [len])
      len++;
    return len;
  }

  #ifndef RK_MEMORY_NO_STD

  template <>
  static inline uptr string_length <char> (const char* s)
  {
    return std::strlen (s);
  }

  template <>
  static inline uptr string_length <wchar_t> (const wchar_t* s)
  {
    return std::wcslen (s);
  }

  #endif

  // Pointer evaluation for various smart pointer types
  template <typename T>
  static inline T* get_pointer (T* p)
  {
    return p;
  }

  template <typename T>
  static inline T* get_pointer (const std::shared_ptr <T>& p)
  {
    return p.get ();
  }

  template <typename T>
  static inline T* get_pointer (const std::unique_ptr <T>& p)
  {
    return p.get ();
  }

  // Fills an object's storage with a recognizable byte pattern
  // Nilpotent in non-debug builds
  template <typename T>
  static inline void clobber (T* obj)
  {
    #ifndef NDEBUG
    memory_fill (obj, sizeof (T), "Rk", 2);
    #endif
  }
  
  //
  // Algorithms
  //
  template <typename OutIter, typename InIter>
  OutIter copy (OutIter dest, InIter src, uptr length)
  {
    while (length--)
      *dest++ = *src++;
    return dest;
  }

  template <typename OutIter, typename InIter>
  OutIter copy (OutIter dest, OutIter limit, InIter begin, InIter end)
  {
    while (dest != limit && begin != end)
      *dest++ = *begin++;
    return dest;
  }
  
  template <typename OutIter, typename InIter>
  OutIter copy (OutIter dest, uptr length, InIter begin, InIter end)
  {
    return copy (dest, dest + length, begin, end);
  }

  template <typename OutIter, typename InIter>
  OutIter copy (OutIter dest, OutIter limit, InIter begin, uptr size)
  {
    return copy (dest, limit, begin, begin + size);
  }

  template <typename OutIter, typename InIter>
  OutIter copy (OutIter dest, uptr length, InIter begin, uptr size)
  {
    return copy (dest, dest + length, begin, begin + size);
  }

  // Container variants
  template <typename OutIter, typename Cont>
  auto copy (OutIter dest, OutIter limit, const Cont& cont)
    -> typename std::enable_if <IsIterable <Cont>::value, OutIter>::type
  {
    auto begin = std::begin (cont);
    auto end   = std::end   (cont);

    return copy (dest, limit, begin, end);
  }

  template <typename OutIter, typename Cont>
  OutIter copy (OutIter dest, uptr length, const Cont& cont)
  {
    return copy (dest, dest + length, cont);
  }

  template <typename T>
  class RawStorage
  {
    // is_trivially_copyable not implemented in MSVC10
    static_assert (std::is_pod <T>::value, "RawStorage may only store trivially copyable types");

    char store [sizeof (T) + __alignof (T) - 1];

  public:
    char* raw () { return align (store, __alignof (T)); }

    T& value () { return *reinterpret_cast <T*> (raw ()); }

  };

} // namespace Rk

using Rk::get_pointer;
