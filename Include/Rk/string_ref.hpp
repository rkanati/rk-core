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

namespace Rk
{
  template <typename unit_t>
  static inline size_t null_terminated_length (const unit_t* ptr, size_t limit = ~size_t (0))
  {
    size_t len = 0;
    while (len < limit && ptr [len])
      len++;
    return len;
  }

  template <typename unit_t>
  class string_ref_base
  {
    const unit_t* ptr;
    size_t        len;

  public:
    typedef unit_t*       iterator;
    typedef const unit_t* const_iterator;

    string_ref_base (nil_t n = nil) :
      ptr (nullptr),
      len (0)
    { }
    
    string_ref_base (const unit_t* new_ptr) :
      ptr (new_ptr),
      len (null_terminated_length (ptr))
    { }
    
    string_ref_base (const unit_t* new_ptr, size_t new_len) :
      ptr (new_ptr),
      len (new_len)
    { }
    
    string_ref_base (const unit_t* begin, const unit_t* end) :
      ptr (begin),
      len (end - ptr)
    { }
    
    template <typename traits_t, typename alloc_t>
    string_ref_base (const std::basic_string <unit_t, traits_t, alloc_t>& s) :
      ptr (s.data ()),
      len (s.length ())
    { }

    bool operator == (const string_ref_base& other) const
    {
      return len == other.len && std::equal (ptr, ptr + len, other.ptr);
    }
    
    bool operator != (const string_ref_base& other) const
    {
      return !(*this == other);
    }
    
    explicit operator bool () const
    {
      return ptr != nullptr;
    }

    const unit_t* data () const
    {
      return ptr;
    }

    size_t length () const
    {
      return len;
    }

    size_t size () const
    {
      return len;
    }

    bool empty () const
    {
      return len == 0;
    }

    const unit_t* begin () const
    {
      return ptr;
    }

    const unit_t* end () const
    {
      return ptr + len;
    }
    
    unit_t at (size_t index) const
    {
      if (index >= len)
        throw std::range_error ("Rk::string_ref_base::at - index out of range");
      else
        return ptr [index];
    }

    unit_t operator [] (size_t index) const
    {
      return ptr [index];
    }

    string_ref_base slice (size_t first, size_t limit) const
    {
      return { ptr + first, ptr + limit };
    }

  };

  template <typename unit_t>
  auto make_string_ref (const unit_t* ptr) 
    -> string_ref_base <unit_t>
  {
    return ptr;
  }
  
  template <typename unit_t, typename arg2_t>
  auto make_string_ref (const unit_t* ptr, arg2_t&& arg2) 
    -> string_ref_base <unit_t>
  {
    return string_ref_base <unit_t> (ptr, std::forward <arg2_t> (arg2));
  }
  
  template <typename unit_t, typename traits_t, typename alloc_t>
  auto make_string_ref (const std::basic_string <unit_t, traits_t, alloc_t>& s)
    -> string_ref_base <unit_t>
  {
    return s;
  }

  template <typename unit_t>
  static inline auto to_string (string_ref_base <unit_t> str)
    -> std::basic_string <unit_t>
  {
    return std::basic_string <unit_t> { str.data (), str.length () };
  }

  template <typename out_stream, typename unit_t>
  out_stream& operator << (out_stream& os, string_ref_base <unit_t> str)
  {
    os.write (str.data (), str.length ());
    return os;
  }

  using cstring_ref   = string_ref_base <char>;
  using wstring_ref   = string_ref_base <wchar>;
  using u16string_ref = string_ref_base <char16>;
  using u32string_ref = string_ref_base <char32>;

}

namespace std
{
  template <typename T>
  struct hash;
  
  template <typename unit_t>
  struct hash <Rk::string_ref_base <unit_t>>
  {
  public:
    typedef Rk::string_ref_base <unit_t> argument_type;
    typedef size_t                       result_type;
    
    result_type operator () (argument_type s) const
    {
      auto ptr = (const u8*) s.data ();
      auto len = s.size () * sizeof (unit_t);

      // DJB hash
      size_t h = 5381;
      while (len--)
        h += (h << 5) + *ptr++;

      return h;
    }
    
  };
  
}
