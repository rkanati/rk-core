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

namespace Rk {
  template<typename Value>
  static constexpr bool is_dense_trivially_copyable
    =   std::is_trivially_copyable_v<Value>
    &&  (sizeof (Value) % std::alignment_of_v<Value> == 0);

  template <typename test>
  struct trait {
    template <typename T>
    struct type :
      std::is_void <decltype (test::test ((T*) 0))>
    { };
  };

  template <typename lhs_t, typename rhs_t>
  using sum_t        = decltype (std::declval <lhs_t> () + std::declval <rhs_t> ());

  template <typename lhs_t, typename rhs_t>
  using difference_t = decltype (std::declval <lhs_t> () - std::declval <rhs_t> ());

  template <typename lhs_t, typename rhs_t>
  using product_t    = decltype (std::declval <lhs_t> () * std::declval <rhs_t> ());

  template <typename lhs_t, typename rhs_t>
  using quotient_t   = decltype (std::declval <lhs_t> () / std::declval <rhs_t> ());

  template <typename lhs_t, typename rhs_t>
  using remainder_t  = decltype (std::declval <lhs_t> () % std::declval <rhs_t> ());

  template <typename lhs_t, typename rhs_t>
  struct /*using*/ const_and_t /*=*/ : std::integral_constant <bool, lhs_t::value && rhs_t::value> { } ;

  namespace traits_private {
    template <typename... ts>
    struct is_same_comp;

    template <typename t>
    struct is_same_comp <t, t> :
      std::true_type
    { };

    template <typename t, typename u>
    struct is_same_comp <t, u> :
      std::false_type
    { };

    template <typename t, typename u, typename... ts>
    struct is_same_comp <t, u, ts...> :
      const_and_t <is_same_comp <t, u>, is_same_comp <u, ts...>>
    { };

    template <typename t, bool same>
    struct is_same_impl :
      std::false_type
    { };

    template <typename t>
    struct is_same_impl <t, true> :
      std::true_type
    {
      typedef t same_t;
    };
  }

  template <typename t, typename... ts>
  using is_same = traits_private::is_same_impl <t, traits_private::is_same_comp <t, ts...>::value>;
}

