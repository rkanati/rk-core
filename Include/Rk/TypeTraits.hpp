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

#include <type_traits>

namespace Rk
{
  template <typename T>
  auto declval () -> typename std::add_rvalue_reference <T>::type;

  template <typename Test>
  struct Trait
  {
    template <typename T>
    struct Type :
      std::is_void <decltype (Test::test ((T*) 0))>
    { };

  };

  namespace TraitsPrivate
  {
    struct Iterable
    {
      template <typename T>
      static void test (
        T* t,
        decltype (std::begin (*t))* b = 0,
        decltype (std::end   (*t))* e = 0/*,
        decltype (**b)* = 0,
        decltype (**e)* = 0,
        decltype (*b == *e)* = 0,
        decltype (++*b)* = 0*/
      );

      static int test (...);

    };

  }

  template <typename T>
  struct IsIterable :
    Trait <TraitsPrivate::Iterable>::Type <T>
  { };

}
