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
#include <Rk/memory.hpp>
#include <Rk/types.hpp>

#include <string>

namespace Rk
{
  template <typename char_t, typename traits = std::char_traits <char_t>>
  class string_ref_base
  {
    const char_t* ptr;
    size_t        len;

  public:
    typedef char_t*       iterator;
    typedef const char_t* const_iterator;

    string_ref_base (nil_t n = nil) :
      ptr (nullptr),
      len (0)
    { }
    
    string_ref_base (const char_t* new_ptr) :
      ptr (new_ptr),
      len (traits::length (ptr))
    { }
    
    template <size_t param_size>
    string_ref_base (const char_t (&str) [param_size]) :
      ptr (str),
      len (param_size - 1)
    { }
    
    string_ref_base (const char_t* new_ptr, size_t new_len) :
      ptr (new_ptr),
      len (new_len)
    { }
    
    string_ref_base (const char_t* begin, const char_t* end) :
      ptr (begin),
      len (end - ptr)
    { }
    
    string_ref_base (const std::basic_string <char_t, traits>& s) :
      ptr (s.data ()),
      len (s.length ())
    { }

    bool operator == (const string_ref_base& other) const
    {
      return len == other.len &&
             traits::compare (ptr, other.ptr, len) == 0;
    }
    
    bool operator != (const string_ref_base& other) const
    {
      return !(*this == other);
    }
    
    explicit operator bool () const
    {
      return ptr != nullptr;
    }

    const char_t* data () const
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

    const char_t* begin () const
    {
      return ptr;
    }

    const char_t* end () const
    {
      return ptr + len;
    }
    
    char_t at (size_t index) const
    {
      if (index >= len)
        throw std::range_error ("Rk::string_ref_base::at - index out of range");
      else
        return ptr [index];
    }

    char_t operator [] (size_t index) const
    {
      return ptr [index];
    }

  };

  template <typename Str>
  struct char_type_impl
  {
    static Str& val ();

    typedef typename std::remove_const_t <
      typename std::remove_reference_t <
        decltype (val () [0])
      >
    > type;

  };

  template <typename Str>
  using char_type = typename char_type_impl <Str>::type;

  template <typename first_t, typename... args>
  auto make_string_ref (first_t&& first, args&&... args)
    -> string_ref_base <char_type <first_t>>
  {
    return {
      std::forward <first> (first),
      std::forward <args>  (args)...
    };
  }

  template <typename char_t, typename traits>
  static inline auto to_string (string_ref_base <char_t, traits> str)
    -> std::basic_string <char_t, traits>
  {
    return { str.data (), str.length () };
  }

  template <typename out_stream, typename char_t, typename traits>
  out_stream& operator << (out_stream& os, string_ref_base <char_t, traits> str)
  {
    os.write (str.data (), str.length ());
    return os;
  }

  using string_ref    = string_ref_base <char>;
  using wstring_ref   = string_ref_base <wchar>;
  using u16string_ref = string_ref_base <char16_t>;
  using u32string_ref = string_ref_base <char32_t>;

  // Convenience for wide literals
  template <typename Char, size_t len>
  u16string_ref u16_string (const Char (&str) [len])
  {
    return u16string_ref (reinterpret_cast <const char16_t (&) [len]> (str));
  }

}

namespace std
{
  template <typename T>
  class hash;
  
  template <typename char_t>
  class hash <Rk::string_ref_base <char_t>>
  {
  public:
    typedef Rk::string_ref_base <char_t> argument_type;
    typedef size_t                       result_type;
    
    result_type operator () (argument_type s) const
    {
      auto ptr = (const u8*) s.data ();
      auto len = s.size () * sizeof (char_t);

      // DJB hash
      size_t h = 5381;
      while (len--)
        h += (h << 5) + *ptr++;

      return h;
    }
    
  };
  
}
