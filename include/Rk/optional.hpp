//
// Copyright (C) 2015 Rk
// Permission to use, copy, modify and distribute this file for any purpose is hereby granted without fee or other
// limitation, provided that
//  - this notice is reproduced verbatim in all copies and modifications of this file, and
//  - neither the names nor trademarks of the copyright holder are used to endorse any work which includes or is derived
//    from this file.
// No warranty applies to the use, either of this software, or any modification of it, and Rk disclaims all liabilities
// in relation to such use.
//

#pragma once

#include <Rk/memory.hpp>

namespace Rk {
  template <typename content_t>
  class optional {
    raw_storage <content_t> store;
    bool                    engaged;

  public:
    optional () :
      engaged (false)
    { }

    optional (const content_t& value) :
      engaged (true)
    {
      new (store.raw ()) content_t (value);
    }

    optional (content_t&& value) :
      engaged (true)
    {
      new (store.raw ()) content_t (std::move (value));
    }

    optional& operator = (const content_t& value) {
      if (engaged) store.value () = value;
      else new (store.raw ()) content_t (value);
      engaged = true;
      return *this;
    }

    optional& operator = (content_t&& value) {
      if (engaged) store.value () = std::move (value);
      else new (store.raw ()) content_t (std::move (value));
      engaged = true;
      return *this;
    }

    optional (const optional& other) :
      engaged (other.engaged)
    {
      if (other.engaged)
        new (store.raw ()) content_t (other.get ());
    }

    optional (optional&& other) :
      engaged (other.engaged)
    {
      if (other.engaged)
        new (store.raw ()) content_t (std::move (other.get ()));
      other.engaged = false;
    }

    optional& operator = (const optional& other) {
      if (engaged) store.value () = other.get ();
      else new (store.raw ()) content_t (other.get ());
      engaged = true;
      return *this;
    }

    optional& operator = (optional&& other) {
      if (engaged) store.value () = std::move (other.get ());
      else new (store.raw ()) content_t (std::move (other.get ()));
      engaged = true;
      other.engaged = false;
      return *this;
    }

    content_t& get () {
      if (!engaged) throw std::logic_error ("optional not engaged");
      return store.value ();
    }

    const content_t& get () const {
      if (!engaged) throw std::logic_error ("optional not engaged");
      return store.value ();
    }

    explicit operator bool () const {
      return engaged;
    }
  };
}

