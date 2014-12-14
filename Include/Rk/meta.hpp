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

namespace Rk {
  template <size_t...>
  struct idx_seq { };

  namespace meta_private {
    template <size_t base, size_t remaining, size_t... rest>
    struct idx_seq_maker :
      idx_seq_maker <base, remaining - 1, base + remaining - 1, rest...>
    { };

    template <size_t base, size_t... rest>
    struct idx_seq_maker <base, 0, rest...> {
      using seq = idx_seq <rest...>;
    };

  }

  template <int base, int n>
  using make_idxs = typename meta_private::idx_seq_maker <base, n - base>::seq;

  template <typename... ts>
  using make_idxs_for = make_idxs <0, sizeof... (ts)>;

}
