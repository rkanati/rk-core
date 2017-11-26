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

#include <algorithm>
#include <string>

namespace Rk {
  template <typename Unit>
  static inline size_t null_terminated_length (Unit const* ptr, size_t limit = ~size_t (0)) {
    size_t len = 0;
    while (len < limit && ptr [len])
      len++;
    return len;
  }

  template <typename Unit>
  class StringRefBase {
    Unit const* ptr;
    size_t      len;

  public:
    typedef Unit*       iterator;
    typedef Unit const* const_iterator;

    StringRefBase () :
      ptr (nullptr),
      len (0)
    { }

    explicit StringRefBase (Unit const& unit) :
      ptr (&unit),
      len (1)
    { }

    StringRefBase (Unit const* new_ptr) :
      ptr (new_ptr),
      len (null_terminated_length (ptr))
    { }

    StringRefBase (Unit const* new_ptr, size_t new_len) :
      ptr (new_ptr),
      len (new_len)
    { }

    StringRefBase (Unit const* begin, Unit const* end) :
      ptr (begin),
      len (end - ptr)
    { }

    template <typename Traits, typename Alloc>
    StringRefBase (std::basic_string <Unit, Traits, Alloc> const& s) :
      ptr (s.data ()),
      len (s.length ())
    { }

    bool operator == (StringRefBase const& other) const {
      return len == other.len && std::equal (ptr, ptr + len, other.ptr);
    }

    bool operator != (StringRefBase const& other) const {
      return !(*this == other);
    }

    explicit operator bool () const {
      return ptr != nullptr;
    }

    Unit const* data () const {
      return ptr;
    }

    size_t length () const {
      return len;
    }

    size_t size () const {
      return len;
    }

    bool empty () const {
      return len == 0;
    }

    Unit const* begin () const {
      return ptr;
    }

    Unit const* cbegin () const {
      return begin ();
    }

    Unit const* end () const {
      return ptr + len;
    }

    Unit const* cend () const {
      return end ();
    }

    Unit at (size_t index) const {
      if (index >= len)
        throw std::range_error ("index out of range");
      else
        return ptr [index];
    }

    Unit operator [] (size_t index) const {
      return ptr [index];
    }

    StringRefBase slice (size_t first, size_t limit) const {
      return { ptr + first, ptr + limit };
    }
  };

  template <typename Unit>
  auto make_string_ref (StringRefBase <Unit> sr) {
    return sr;
  }

  template <typename Unit>
  auto make_string_ref (Unit const* ptr) -> StringRefBase <Unit> {
    return ptr;
  }

  template <typename Unit, typename Arg2>
  auto make_string_ref (Unit const* ptr, Arg2&& arg2) -> StringRefBase <Unit> {
    return StringRefBase <Unit> (ptr, std::forward <Arg2> (arg2));
  }

  template <typename Unit, typename Traits, typename Alloc>
  auto make_string_ref (std::basic_string <Unit, Traits, Alloc> const& s) -> StringRefBase <Unit> {
    return s;
  }

  template <typename Unit>
  static inline auto to_string (StringRefBase <Unit> str) -> std::basic_string <Unit> {
    return std::basic_string <Unit> { str.data (), str.length () };
  }

  template <typename OutStream, typename Unit>
  OutStream& operator << (OutStream& stream, StringRefBase <Unit> str) {
    stream.write (str.data (), str.length ());
    return stream;
  }

  using StringRef    = StringRefBase <char>;
  using WStringRef   = StringRefBase <wchar>;
  using U16StringRef = StringRefBase <char16>;
  using U32StringRef = StringRefBase <char32>;
}

namespace std {
  template <typename T>
  struct hash;

  template <typename Unit>
  struct hash <Rk::StringRefBase <Unit>> {
  public:
    typedef Rk::StringRefBase <Unit> argument_type;
    typedef size_t                   result_type;

    result_type operator () (argument_type s) const {
      auto ptr = (u8 const*) s.data ();
      auto len = s.size () * sizeof (Unit);

      // DJB hash
      size_t h = 5381;
      while (len--)
        h += (h << 5) + *ptr++;
      return h;
    }
  };
}

