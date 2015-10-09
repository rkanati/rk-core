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

#include <iostream>

namespace RkTest {
  extern int depth, prev;

  static inline void test_begin (const char* msg) {
    if (depth != prev)
      std::cout << '\n';

    for (int i = 0; i != depth; i++)
      std::cout << "  ";

    std::cout << msg << " - ";

    prev = depth++;
  }

  static inline bool test_end (bool& ok, bool pass) {
    if (--depth != prev) {
      for (int i = 0; i != depth; i++)
        std::cout << "  ";
    }

    prev = depth;

    if (pass) std::cout << "pass\n";
    else      std::cout << "fail\n";

    ok = ok && pass;
    return pass;
  }

  template <typename Fn>
  static inline bool test (bool& ok, const char* msg, Fn fn) {
    test_begin (msg);

    bool pass;

    try {
      pass = fn ();
    }
    catch (...) {
      pass = false;
    }

    return test_end (ok, pass);
  }
}

#define TEST(expr) \
  test ( \
    ok, \
    #expr, \
    [=] () -> bool { \
      return (expr); \
    } \
  )

#define TEST_EXCEPT(expr, E) \
  test ( \
    ok, \
    #expr " throws " #E, \
    [=] () -> bool { \
      try { (expr); return false; } catch (E&) { return true; } catch (...) { throw; } \
    } \
  )

