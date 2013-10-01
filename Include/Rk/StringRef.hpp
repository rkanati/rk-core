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
#include <Rk/Memory.hpp>
#include <Rk/Types.hpp>

#include <string>

namespace Rk
{
  template <typename Char, typename Traits = std::char_traits <Char>>
  class StringRefBase
  {
    const Char* ptr;
    size_t      len;

  public:
    typedef Char*       iterator;
    typedef const Char* const_iterator;

    StringRefBase (Nil n = nil) :
      ptr (nullptr),
      len (0)
    { }
    
    StringRefBase (const Char* new_ptr) :
      ptr (new_ptr),
      len (Traits::length (ptr))
    { }
    
    template <size_t param_size>
    StringRefBase (const Char (&str) [param_size]) :
      ptr (str),
      len (param_size - 1)
    { }
    
    StringRefBase (const Char* new_ptr, size_t new_len) :
      ptr (new_ptr),
      len (new_len)
    { }
    
    StringRefBase (const Char* begin, const Char* end) :
      ptr (begin),
      len (end - ptr)
    { }
    
    StringRefBase (const std::basic_string <Char, Traits>& s) :
      ptr (s.data ()),
      len (s.length ())
    { }

    bool operator == (const StringRefBase& other) const
    {
      return len == other.len &&
             Traits::compare (ptr, other.ptr, len) == 0;
    }
    
    bool operator != (const StringRefBase& other) const
    {
      return !(*this == other);
    }
    
    explicit operator bool () const
    {
      return ptr != nullptr;
    }

    const Char* data () const
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

    const Char* begin () const
    {
      return ptr;
    }

    const Char* end () const
    {
      return ptr + len;
    }
    
    Char at (size_t index) const
    {
      if (index >= len)
        throw std::range_error ("Rk::StringRefBase::at - index out of range");
      else
        return ptr [index];
    }

    Char operator [] (size_t index) const
    {
      return ptr [index];
    }

  };

  template <typename Str>
  struct StrCharType
  {
    static Str& val ();

    typedef typename std::remove_const <
      typename std::remove_reference <
        decltype (val () [0])
      >::type
    >::type Type;

  };

  template <typename First, typename... Args>
  auto make_stringref (First&& first, Args&&... args)
    -> StringRefBase <typename StrCharType <First>::Type>
  {
    return StringRefBase <typename StrCharType <First>::Type> (
      std::forward <First> (first),
      std::forward <Args>  (args)...
    );
  }

  template <typename Char, typename Traits>
  static inline std::basic_string <Char, Traits> to_string (StringRefBase <Char, Traits> str)
  {
    return std::basic_string <Char, Traits> (str.data (), str.length ());
  }

  template <typename OS, typename Char, typename Traits>
  OS& operator << (OS& os, StringRefBase <Char, Traits> str)
  {
    os.write (str.data (), str.length ());
    return os;
  }

  using StringRef    = StringRefBase <char>;
  using WStringRef   = StringRefBase <wchar>;
  using U16StringRef = StringRefBase <char16_t>;
  using U32StringRef = StringRefBase <char32_t>;

  // Convenience for wide literals
  template <typename Char, size_t len>
  U16StringRef u16_string (const Char (&str) [len])
  {
    return U16StringRef (reinterpret_cast <const char16_t (&) [len]> (str));
  }

}

namespace std
{
  template <typename T>
  class hash;
  
  template <typename Char>
  class hash <Rk::StringRefBase <Char>>
  {
  public:
    typedef Rk::StringRefBase <Char> argument_type;
    typedef size_t                   result_type;
    
    result_type operator () (argument_type s) const
    {
      auto ptr = (const u8*) s.data ();
      auto len = s.size () * sizeof (Char);

      // DJB hash
      size_t h = 5381;
      while (len--)
        h += (h << 5) + *ptr++;

      return h;
    }
    
  };
  
}
