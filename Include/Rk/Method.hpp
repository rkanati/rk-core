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

namespace Rk
{
  //
  // = method ==========================================================================================================
  // Creates forwarding functors to methods on bound targets.
  //

#if defined (_MSC_VER) && (_MSC_VER < 1800)

  template <typename P, typename R, typename C>
  class Method0
  {
    P p;
    R (C::*f) ();

  public:
    Method0 (P p, R (C::*f) ()) :
      p (std::move (p)),
      f (f)
    { }
    
    R operator () ()
    {
      return (get_pointer (p) ->* f) ();
    }

  };

  template <typename P, typename R, typename C, typename A0>
  class Method1
  {
    P p;
    R (C::*f) (A0);

  public:
    Method1 (P p, R (C::*f) (A0)) :
      p (std::move (p)),
      f (f)
    { }
    
    template <typename CA0>
    R operator () (CA0&& a0)
    {
      return (get_pointer (p) ->* f) (
        std::forward <CA0> (a0)
      );
    }

  };

  template <typename P, typename R, typename C, typename A0, typename A1>
  class Method2
  {
    P p;
    R (C::*f) (A0, A1);

  public:
    Method2 (P p, R (C::*f) (A0, A1)) :
      p (std::move (p)),
      f (f)
    { }
    
    template <typename CA0, typename CA1>
    R operator () (CA0&& a0, CA1&& a1)
    {
      return (get_pointer (p) ->* f) (
        std::forward <CA0> (a0),
        std::forward <CA1> (a1)
      );
    }

  };

  template <typename P, typename R, typename C, typename A0, typename A1, typename A2>
  class Method3
  {
    P p;
    R (C::*f) (A0, A1, A2);

  public:
    Method3 (P p, R (C::*f) (A0, A1, A2)) :
      p (std::move (p)),
      f (f)
    { }
    
    template <typename CA0, typename CA1, typename CA2>
    R operator () (CA0&& a0, CA1&& a1, CA2&& a2)
    {
      return (get_pointer (p) ->* f) (
        std::forward <CA0> (a0),
        std::forward <CA1> (a1),
        std::forward <CA2> (a2)
      );
    }

  };

  template <typename P, typename R, typename C, typename A0, typename A1, typename A2, typename A3>
  class Method4
  {
    P p;
    R (C::*f) (A0, A1, A2, A3);

  public:
    Method4 (P p, R (C::*f) (A0, A1, A2, A3)) :
      p (std::move (p)),
      f (f)
    { }
    
    template <typename CA0, typename CA1, typename CA2, typename CA3>
    R operator () (CA0&& a0, CA1&& a1, CA2&& a2, CA3&& a3)
    {
      return (get_pointer (p) ->* f) (
        std::forward <CA0> (a0),
        std::forward <CA1> (a1),
        std::forward <CA2> (a2),
        std::forward <CA3> (a3)
      );
    }

  };

  template <typename P, typename R, typename C, typename A0, typename A1, typename A2, typename A3, typename A4>
  class Method5
  {
    P p;
    R (C::*f) (A0, A1, A2, A3, A4);

  public:
    Method5 (P p, R (C::*f) (A0, A1, A2, A3, A4)) :
      p (std::move (p)),
      f (f)
    { }
    
    template <typename CA0, typename CA1, typename CA2, typename CA3, typename CA4>
    R operator () (CA0&& a0, CA1&& a1, CA2&& a2, CA3&& a3, CA4&& a4)
    {
      return (get_pointer (p) ->* f) (
        std::forward <CA0> (a0),
        std::forward <CA1> (a1),
        std::forward <CA2> (a2),
        std::forward <CA3> (a3),
        std::forward <CA4> (a4)
      );
    }

  };

  template <typename P, typename R, typename C>
  Method0 <P, R, C> method_ (P p, R (C::*f) ())
  {
    return Method0 <P, R, C> (std::move (p), f);
  }

  template <typename P, typename R, typename C, typename A0>
  Method1 <P, R, C, A0> method_ (P p, R (C::*f) (A0))
  {
    return Method1 <P, R, C, A0> (std::move (p), f);
  }

  template <typename P, typename R, typename C, typename A0, typename A1>
  Method2 <P, R, C, A0, A1> method_ (P p, R (C::*f) (A0, A1))
  {
    return Method2 <P, R, C, A0, A1> (std::move (p), f);
  }

  template <typename P, typename R, typename C, typename A0, typename A1, typename A2>
  Method3 <P, R, C, A0, A1, A2> method_ (P p, R (C::*f) (A0, A1, A2))
  {
    return Method3 <P, R, C, A0, A1, A2> (std::move (p), f);
  }

  template <typename P, typename R, typename C, typename A0, typename A1, typename A2, typename A3>
  Method4 <P, R, C, A0, A1, A2, A3> method_ (P p, R (C::*f) (A0, A1, A2, A3))
  {
    return Method4 <P, R, C, A0, A1, A2, A3> (std::move (p), f);
  }

  template <typename P, typename R, typename C, typename A0, typename A1, typename A2, typename A3, typename A4>
  Method5 <P, R, C, A0, A1, A2, A3, A4> method_ (P p, R (C::*f) (A0, A1, A2, A3, A4))
  {
    return Method5 <P, R, C, A0, A1, A2, A3, A4> (std::move (p), f);
  }

#else
  
  template <typename Ptr, typename Result, typename Target, typename... Args>
  class Method
  {
  public:
    typedef Result (Target::*Method) (Args...);

  private:
    Ptr    ptr;
    Method method;

  public:
    AsyncMethod (Ptr new_ptr, Method new_method) :
      weak   (std::move (new_ptr)),
      method (new_method)
    { }
    
    template <typename... ArgVals>
    Result operator () (ArgVals&&... vals)
    {
      return (get_pointer (ptr) ->* method) (std::forward <ArgVals...> (vals...);
    }

  };

  template <typename Ptr, typename Result, typename Target, typename... Args>
  Method <Ptr, Result, Target, Args...> method_proxy (Ptr ptr, Result (Target::*method) (Args...))
  {
    return Method <Ptr, Result, Target, Args...> (std::move (p), method);
  }

#endif
  
} // namespace Rk
