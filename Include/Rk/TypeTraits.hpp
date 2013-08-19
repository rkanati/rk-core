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
  template <typename T, typename Test>
  struct TypeTrait :
    public std::conditional <
      std::is_void <decltype (Test::test ((T*) 0))>::value,
      std::true_type,
      std::false_type
    >::type
  { };

  struct IsIterableTest
  {
    template <typename T>
    static void test (
      T* p,
      decltype (std::begin (*p))* = 0,
      decltype (std::end   (*p))* = 0
    );

    static int test (...);

  };

  template <typename T>
  struct IsIterable :
    TypeTrait <T, IsIterableTest>
  { };

}
