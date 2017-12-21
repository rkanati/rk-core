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

#include <utility>

namespace Rk {
  template <typename Func>
  class Guard {
    Func func;
    bool active;

  public:
    explicit Guard (Func new_func) :
      func   (new_func),
      active (true)
    { }

    Guard             (const Guard&) = delete;
    Guard& operator = (const Guard&) = delete;

    Guard (Guard&& other) :
      func   (std::move (other.func)),
      active (other.active)
    {
      other.active = false;
    }

    Guard& operator = (Guard&& other) {
      func = std::move (other.func);
      std::swap (active = false, other.active);
      return *this;
    }

    ~Guard () {
      if (active)
        func ();
    }

    void relieve () {
      active = false;
    }
  };

  template <typename Func>
  auto guard (Func func)
    -> Guard <Func>
  {
    return Guard <Func> (func);
  }

  template <typename Func, typename... Args>
  auto guard (Func func, Args... args) {
    return guard ([func, args...] { func (args...); });
  }

  template <typename T>
  auto guard_delete (T* ptr) {
    return guard ([ptr] { delete ptr; });
  }

  template <typename T>
  auto guard_array (T* arr) {
    return guard ([arr] { delete [] arr; });
  }

  void relieve () { }

  template <typename func_t, typename... func_ts>
  void relieve (guard_t <func_t>& g, guard_t <func_ts>&... gs) {
    g.relieve ();
    relieve (gs...);
  }
}

