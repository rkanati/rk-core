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

namespace Rk
{
  template <typename func_t>
  class guard_t
  {
    func_t func;
    bool   active;

  public:
    explicit guard_t (func_t new_func) :
      func   (std::move (new_func)),
      active (true)
    { }
    
    guard_t             (const guard_t&) = delete;
    guard_t& operator = (const guard_t&) = delete;

    guard_t (guard_t&& other) :
      func   (std::move (other.func)),
      active (other.active)
    {
      other.active = false;
    }
    
    guard_t& operator = (guard_t&& other)
    {
      func = std::move (other.func);
      std::swap (active = false, other.active);
      return *this;
    }

    ~guard_t ()
    {
      if (active)
        func ();
    }

    void relieve ()
    {
      active = false;
    }

  };

  template <typename func_t>
  auto guard (func_t func)
    -> guard_t <func_t>
  {
    return guard_t <func_t> (std::move (func));
  }

}
