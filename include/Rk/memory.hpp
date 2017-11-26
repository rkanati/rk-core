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

namespace Rk {
  // Returns the least multiple of alignment greater or equal to value
  template <typename T, typename U>
  static inline T align (T value, U alignment) {
    auto const mod = value % alignment;
    return mod ? value + (alignment - mod) : value;
  }

  template <typename T, typename U>
  static inline T* align (T* p, U alignment) {
    return reinterpret_cast <T*> (align (reinterpret_cast <uptr> (p), alignment));
  }

  // Faster alternative to align for power-of-two alignments
  template <typename T, typename U>
  static inline T align2 (T value, U alignment) {
    return (value + alignment - 1) & (alignment - 1);
  }

  template <typename T, typename U>
  static inline T* align2 (T* p, U alignment) {
    return reinterpret_cast <T*> (align2 (reinterpret_cast <uptr> (p), alignment));
  }

  // Returns the index'th T in the array starting at ptr where each consecutive pair of elements
  // start at addresses stride bytes apart
  template <typename T>
  static inline T& sparse_get (T* ptr, uptr stride, uptr index) {
    return *(T*) ((u8*) ptr + stride * index);
  }

  template <typename T>
  static inline T const& sparse_get (T const* ptr, uptr stride, uptr index) {
    return *(T const*) ((u8 const*) ptr + stride * index);
  }

  // Pointer evaluation for various smart pointer types
  template <typename T>
  static inline T* get_pointer (T* p) {
    return p;
  }

  template <typename T>
  static inline T* get_pointer (std::shared_ptr const <T>& p) {
    return p.get ();
  }

  template <typename T>
  static inline T* get_pointer (std::unique_ptr const <T>& p) {
    return p.get ();
  }

  // copy, counted
  template <typename OutIterator, typename InIterator>
  OutIterator copy (OutIterator dest, InIterator src, size_t length) {
    while (length--)
      *dest++ = *src++;
    return dest;
  }

  // dense copy, counted
  template <typename T, typename = std::enable_if_t <is_dense_trivially_copyable <T>>>
  T* copy (T* dest, T const* src, size_t length) {
    memcpy (dest, src, length * sizeof (T));
    return dest + length;
  }

  // copy, limited
  template <typename OutIterator, typename InIterator>
  OutIterator copy (OutIterator dest, OutIterator limit, InIterator begin, InIterator end) {
    while (dest != limit && begin != end)
      *dest++ = *begin++;
    return dest;
  }

  // dense copy, limited
  template <typename T, typename = std::enable_if_t <is_dense_trivially_copyable <T>>>
  T* copy (T* dest, T* limit, T const* begin, T const* end) {
    return Rk::copy (dest, begin, std::min (limit - dest, end - begin));
  }

  // copy, capped and limited
  template <typename OutIterator, typename InIterator>
  OutIterator copy (OutIterator dest, size_t length, InIterator begin, InIterator end) {
    return Rk::copy (dest, dest + length, begin, end);
  }

  template <typename OutIterator, typename InIterator>
  OutIterator copy (OutIterator dest, OutIterator limit, InIterator begin, size_t size) {
    return Rk::copy (dest, limit, begin, begin + size);
  }

  template <typename OutIterator, typename InIterator>
  OutIterator copy (OutIterator dest, size_t length, InIterator begin, size_t size) {
    return Rk::copy (dest, dest + length, begin, begin + size);
  }

  // Container variants
  template <typename OutIterator, typename Container>
  OutIterator copy (OutIterator dest, OutIterator limit, Container const& cont) {
    auto begin = std::begin (cont);
    auto end   = std::end   (cont);

    return Rk::copy (dest, limit, begin, end);
  }

  template <typename OutIterator, typename Container>
  OutIterator copy (OutIterator dest, size_t length, Container const& cont) {
    return Rk::copy (dest, dest + length, cont);
  }

  template <typename Contents, size_t size = sizeof (Contents)>
  class RawStorage {
    static_assert (std::is_trivially_copyable_v <T>, "raw_storage may only store trivially copyable types");
    static_assert (size >= sizeof (Contents), "raw_storage size may not be smaller than sizeof (Contents)");

    std::aligned_storage_t <size, alignof (Contents)>
      store;

  public:
    static const size_t capacity = size;

    void*       raw ()       { return &store; }
    void const* raw () const { return &store; }

    Contents& value () {
      return *reinterpret_cast <Contents*> (raw ());
    }

    Contents const& value () const {
      return *reinterpret_cast <Contents const*> (raw ());
    }
  };
}

