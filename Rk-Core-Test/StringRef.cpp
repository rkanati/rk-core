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

// Tests
#include <Rk/StringRef.hpp>

// Uses
#include <string>
#include <vector>

bool test_stringref ()
{
  auto ref  = Rk::make_stringref (L"test");
  auto ref2 = Rk::make_stringref (std::string ("test 2"));

  const char* p3 = "test 3";
  auto ref3 = Rk::make_stringref (p3);

  std::vector <char16> v4;
  auto ref4 = Rk::make_stringref (v4.data (), v4.size ());

  return true;
}
