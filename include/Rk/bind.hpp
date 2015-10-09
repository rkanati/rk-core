//
// Copyright (C) 2014 Rk
// Permission to use, copy, modify and distribute this file for any purpose is hereby granted without fee or other
// limitation, provided that
//  - this notice is reproduced verbatim in all copies and modifications of this file, and
//  - neither the names nor trademarks of the copyright holder are used to endorse any work which includes or is derived
//    from this file.
// No warranty applies to the use, either of this software, or any modification of it, and Rk disclaims all liabilities
// in relation to such use.
//

#pragma once

#include <Rk/apply.hpp>

namespace Rk {
  template <typename fn_t, typename... b_ts>
  class bind_right_t {
    fn_t                 fn;
    std::tuple <b_ts...> bound_args;

  public:
    bind_right_t (fn_t fn, b_ts... bs) :
      fn (fn), bound_args (bs...)
    { }

    template <typename... a_ts>
    auto operator () (a_ts... as) const {
      return apply (fn, std::tuple_cat (std::make_tuple (as...), bound_args));
    }
  };

  template <typename fn_t, typename... b_ts>
  auto bind_right (fn_t fn, b_ts... bs) {
    return bind_right_t <fn_t, b_ts...> { fn, bs... };
  }

  template <typename fn_t, typename... b_ts>
  class bind_left_t {
    fn_t                 fn;
    std::tuple <b_ts...> bound_args;

  public:
    bind_left_t (fn_t fn, b_ts... bs) :
      fn (fn), bound_args (bs...)
    { }

    template <typename... a_ts>
    auto operator () (a_ts... as) const {
      return apply (fn, std::tuple_cat (bound_args, std::make_tuple (as...)));
    }
  };

  template <typename fn_t, typename... b_ts>
  auto bind_left (fn_t f, b_ts... bs) {
    return bind_left_t <fn_t, b_ts...> { f, bs... };
  }
}

