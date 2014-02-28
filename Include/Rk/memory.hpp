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

#include <Rk/type_traits.hpp>
#include <Rk/types.hpp>

#include <algorithm>
#include <memory>

namespace Rk
{
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
  
  template <typename out_iter, typename in_iter>
  out_iter copy (out_iter dest, in_iter src, size_t length)
  {
    while (length--)
      *dest++ = *src++;
    return dest;
  }

  template <typename out_iter, typename in_iter>
  out_iter copy (out_iter dest, out_iter limit, in_iter begin, in_iter end)
  {
    while (dest != limit && begin != end)
      *dest++ = *begin++;
    return dest;
  }
  
  template <typename out_iter, typename in_iter>
  out_iter copy (out_iter dest, size_t length, in_iter begin, in_iter end)
  {
    return Rk::copy (dest, dest + length, begin, end);
  }

  template <typename out_iter, typename in_iter>
  out_iter copy (out_iter dest, out_iter limit, in_iter begin, size_t size)
  {
    return Rk::copy (dest, limit, begin, begin + size);
  }

  template <typename out_iter, typename in_iter>
  out_iter copy (out_iter dest, size_t length, in_iter begin, size_t size)
  {
    return Rk::copy (dest, dest + length, begin, begin + size);
  }

  // Container variants
  template <typename out_iter, typename cont_t>
  out_iter copy (out_iter dest, out_iter limit, const cont_t& cont)
  {
    auto begin = std::begin (cont);
    auto end   = std::end   (cont);

    return Rk::copy (dest, limit, begin, end);
  }

  template <typename out_iter, typename cont_t>
  out_iter copy (out_iter dest, size_t length, const cont_t& cont)
  {
    return Rk::copy (dest, dest + length, cont);
  }

  template <typename content_t>
  class raw_storage
  {
  //static_assert (std::is_trivially_copyable <T>::value, "raw_storage may only store trivially copyable types");

    std::aligned_storage_t <sizeof (content_t), alignof (content_t)>
      store;

  public:
    void* raw () { return &store; }

    content_t& value () { return *reinterpret_cast <content_t*> (raw ()); }

  };

}
