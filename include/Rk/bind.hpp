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

#include <tuple>
#include <experimental/tuple> // std::apply

namespace Rk {
  template <typename Fn, typename... BoundArgs>
  class BindRight {
    Fn                       fn;
    std::tuple<BoundArgs...> bound_args;

  public:
    explicit BindRight (Fn fn, BoundArgs... bs) :
      fn (fn), bound_args (bs...)
    { }

    template <typename... Args>
    auto operator () (Args... as) const {
      return std::experimental::apply (
        fn,
        std::tuple_cat (std::forward_as_tuple (std::forward<Args> (as)...), bound_args)
      );
    }
  };

  template <typename Fn, typename... BoundArgs>
  auto bind_right (Fn&& fn, BoundArgs... bs) {
    return BindRight <Fn, BoundArgs...> { std::forward<Fn> (fn), bs... };
  }

  template <typename Fn, typename... BoundArgs>
  class BindLeft {
    Fn                        fn;
    std::tuple <BoundArgs...> bound_args;

  public:
    explicit BindLeft (Fn fn, BoundArgs... bs) :
      fn (fn), bound_args (bs...)
    { }

    template <typename... Args>
    auto operator () (Args... as) const {
      return std::experimental::apply (
        fn,
        std::tuple_cat (bound_args, std::forward_as_tuple (std::forward<Args> (as)...))
      );
    }
  };

  template <typename Fn, typename... BoundArgs>
  auto bind_left (Fn&& fn, BoundArgs... bs) {
    return BindLeft <Fn, BoundArgs...> { std::forward<Fn> (fn), bs... };
  }
}

