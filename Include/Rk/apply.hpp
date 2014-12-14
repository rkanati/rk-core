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

#include <Rk/meta.hpp>

#include <tuple>

namespace Rk {
  namespace apply_private {
    template <typename F, int... is, typename... As>
    auto apply_impl (F f, idx_seq <is...>, std::tuple <As...> as) {
      return f (std::get <is> (as)...);
    }

  }

  template <typename F, typename... As>
  auto apply (F f, std::tuple <As...> as) {
    return apply_private::apply_impl (f, make_idxs_for <As...> (), as);
  }

}
