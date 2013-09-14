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

#ifndef RK_STRINGREF_NO_STDSTRING
  #include <string>
#endif

namespace Rk
{
  template <typename Char>
  class StringRefBase
  {
    const Char* ptr;
    uptr        len;
    
    typedef void (StringRefBase::*SafeBool) () const;
    void safe_true () const { };

  public:
    typedef Char*       iterator;
    typedef const Char* const_iterator;

    StringRefBase (Nil n = nil) :
      ptr (0),
      len (0)
    { }
    
    StringRefBase (const Char* p) :
      ptr (p),
      len (string_length (p))
    { }
    
    template <uint param_size>
    StringRefBase (const Char (&str) [param_size]) :
      ptr (str),
      len (param_size - 1)
    { }
    
    StringRefBase (const Char* p, uptr len) :
      ptr (p),
      len (len)
    { }
    
    StringRefBase (const Char* b, const Char* e) :
      ptr (b),
      len (e - b)
    { }
    
    #ifndef RK_STRINGREF_NO_STDSTRING
    
    StringRefBase (const std::basic_string <Char>& s) :
      ptr (s.data ()),
      len (s.length ())
    { }
    
    #endif
    
    StringRefBase& operator = (Nil)
    {
      ptr = 0;
      len = 0;
      return *this;
    }

    StringRefBase& operator = (const StringRefBase& other)
    {
      ptr = other.ptr;
      len = other.len;
      return *this;
    }

    StringRefBase& operator = (const char* str)
    {
      ptr = str;
      len = string_length (str);
      return *this;
    }

    template <uint param_size>
    StringRefBase& operator = (const Char (&array) [param_size])
    {
      ptr = array;
      len = param_size - 1;
      return *this;
    }

    bool operator == (const StringRefBase& other) const
    {
      return len == other.len && memory_equal (ptr, other.ptr, len);
    }
    
    bool operator != (const StringRefBase& other) const
    {
      return !(*this == other);
    }
    
    operator SafeBool () const
    {
      return (ptr && len) ? &StringRefBase::safe_true : 0;
    }

    const Char* data   () const { return ptr; }
    uptr        length () const { return len; }
    uptr        size   () const { return len; }
    bool        empty  () const { return len == 0; }

    const Char* begin () const { return ptr; }
    const Char* end   () const { return ptr + len; }
    
    Char at (uint index) const
    {
      if (index >= len)
        throw std::range_error ("Rk::StringRefBase::at - index out of range");
      else
        return ptr [index];
    }

    Char operator [] (uint index) const
    {
      return ptr [index];
    }

    template <typename Iter>
    Iter append_to (Iter dest, uptr cap) const
    {
      if (cap < len + 1)
        throw std::length_error ("Rk::StringRefBase::append_to - destination buffer too short");

      dest = std::copy (ptr, ptr + len, dest);
      *dest = Char ();

      return dest;
    }

    template <typename Iter>
    Iter append_to (Iter dest, Iter end) const
    {
      return append_to (dest, end - dest);
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

  template <typename Arg>
  auto make_stringref (Arg&& arg)
    -> StringRefBase <typename StrCharType <Arg>::Type>
  {
    return StringRefBase <typename StrCharType <Arg>::Type> (
      std::forward <Arg> (arg)
    );
  }

  template <typename Arg0, typename Arg1>
  auto make_stringref (Arg0&& arg0, Arg1&& arg1)
    -> StringRefBase <typename StrCharType <Arg0>::Type>
  {
    return StringRefBase <typename StrCharType <Arg0>::Type> (
      std::forward <Arg0> (arg0),
      std::forward <Arg1> (arg1)
    );
  }

  #ifndef RK_STRINGREF_NO_STDSTRING

  template <typename Char>
  static inline std::basic_string <Char> to_string (StringRefBase <Char> str)
  {
    return std::basic_string <Char> (str.data (), str.length ());
  }

  #endif
  
  template <typename OS, typename Char>
  OS& operator << (OS& os, StringRefBase <Char> str)
  {
    os.write (str.data (), str.length ());
    return os;
  }

  typedef StringRefBase <char>     StringRef;
  typedef StringRefBase <wchar_t>  WStringRef;
  typedef StringRefBase <char16_t> U16StringRef;
  typedef StringRefBase <char32_t> U32StringRef;

  // Convenience for wide literals
  template <typename Char, uint len>
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
      result_type h = 927;
      for (auto p = s.begin (); p != s.end (); p++)
        h += (h >> 8) + (h << (8 + *p)) + *p;
      return h;
    }
    
  };
  
}
