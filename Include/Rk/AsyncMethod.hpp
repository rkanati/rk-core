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

#include <Rk/Exception.hpp>

#include <utility>
#include <memory>

namespace Rk
{
  //
  // = async_method ====================================================================================================
  // Creates a callback functor similar to Rk::method, but it carries weak pointer to its target, which it then
  // tries to lock at call time, just before forwarding. If the target has expired, an exception is thrown.
  // To allow transient active objects to manage their own lifetimes, it also passes the locked pointer as the first
  // argument to the target method.
  //

  // lock_pointer - Acquire strong reference from weak reference
  // MUST return a valid strong reference or throw an exception
  template <typename C>
  std::shared_ptr <C> lock_pointer (const std::weak_ptr <C>& weak)
  {
    return std::shared_ptr <C> (weak);
  }

  // unlock_pointer - Acquire weak reference from strong reference
  // If strong is empty, MUST throw an exception
  template <typename C>
  std::weak_ptr <C> unlock_pointer (const std::shared_ptr <C>& strong)
  {
    if (!strong)
      throw std::invalid_argument ("Target is null");
    return std::weak_ptr <C> (strong);
  }

  template <typename Strong>
  struct WeakEquiv
  {
    typedef decltype (unlock_pointer (*((Strong*) nullptr))) Type;
  };

#if defined (_MSC_VER) && (_MSC_VER < 1800)

  template <typename Strong, typename Result, typename Target>
  class AsyncMethod0
  {
    typedef typename WeakEquiv <Strong>::Type Weak;

  public:
    typedef Result (Target::*Method) (Strong&);

  private:
    Weak   weak;
    Method method;

  public:
    AsyncMethod0 (const Strong& strong, Method method) :
      weak   (unlock_pointer (strong)),
      method (method)
    { }
    
    Result operator () ()
    {
      auto strong = lock_pointer (weak);
      return (get_pointer (strong) ->* method) (
        strong
      );
    }

  };

  template <typename Strong, typename Result, typename Target, typename A0>
  class AsyncMethod1
  {
    typedef typename WeakEquiv <Strong>::Type Weak;

  public:
    typedef Result (Target::*Method) (Strong&, A0);

  private:
    Weak   weak;
    Method method;

  public:
    AsyncMethod1 (const Strong& strong, Method method) :
      weak   (unlock_pointer (strong)),
      method (method)
    { }
    
    template <typename CA0>
    Result operator () (CA0&& a0)
    {
      auto strong = lock_pointer (weak);
      return (get_pointer (strong) ->* method) (
        strong,
        std::forward <CA0> (a0)
      );
    }

  };

  template <typename Strong, typename Result, typename Target, typename A0, typename A1>
  class AsyncMethod2
  {
    typedef typename WeakEquiv <Strong>::Type Weak;

  public:
    typedef Result (Target::*Method) (Strong&, A0, A1);

  private:
    Weak   weak;
    Method method;

  public:
    AsyncMethod2 (const Strong& strong, Method method) :
      weak   (unlock_pointer (strong)),
      method (method)
    { }
    
    template <typename CA0, typename CA1>
    Result operator () (CA0&& a0, CA1&& a1)
    {
      auto strong = lock_pointer (weak);
      return (get_pointer (strong) ->* method) (
        strong,
        std::forward <CA0> (a0),
        std::forward <CA1> (a1)
      );
    }

  };

  template <typename Strong, typename Result, typename Target, typename A0, typename A1, typename A2>
  class AsyncMethod3
  {
    typedef typename WeakEquiv <Strong>::Type Weak;

  public:
    typedef Result (Target::*Method) (Strong&, A0, A1, A2);

  private:
    Weak   weak;
    Method method;

  public:
    AsyncMethod3 (const Strong& strong, Method method) :
      weak   (unlock_pointer (strong)),
      method (method)
    { }
    
    template <typename CA0, typename CA1, typename CA2>
    Result operator () (CA0&& a0, CA1&& a1, CA2&& a2)
    {
      auto strong = lock_pointer (weak);
      return (get_pointer (strong) ->* method) (
        strong,
        std::forward <CA0> (a0),
        std::forward <CA1> (a1),
        std::forward <CA2> (a2)
      );
    }

  };

  template <typename Strong, typename Result, typename Target, typename A0, typename A1, typename A2, typename A3>
  class AsyncMethod4
  {
    typedef typename WeakEquiv <Strong>::Type Weak;

  public:
    typedef Result (Target::*Method) (Strong&, A0, A1, A2, A3);

  private:
    Weak   weak;
    Method method;

  public:
    AsyncMethod4 (const Strong& strong, Method method) :
      weak   (unlock_pointer (strong)),
      method (method)
    { }
    
    template <typename CA0, typename CA1, typename CA2, typename CA3>
    Result operator () (CA0&& a0, CA1&& a1, CA2&& a2, CA3&& a3)
    {
      auto strong = lock_pointer (weak);
      return (get_pointer (strong) ->* method) (
        strong,
        std::forward <CA0> (a0),
        std::forward <CA1> (a1),
        std::forward <CA2> (a2),
        std::forward <CA3> (a3)
      );
    }

  };

  template <typename Strong, typename Result, typename Target, typename A0, typename A1, typename A2, typename A3, typename A4>
  class AsyncMethod5
  {
    typedef typename WeakEquiv <Strong>::Type Weak;

  public:
    typedef Result (Target::*Method) (Strong&, A0, A1, A2, A3, A4);

  private:
    Weak   weak;
    Method method;

  public:
    AsyncMethod5 (const Strong& strong, Method method) :
      weak   (unlock_pointer (strong)),
      method (method)
    { }
    
    template <typename CA0, typename CA1, typename CA2, typename CA3, typename CA4>
    Result operator () (CA0&& a0, CA1&& a1, CA2&& a2, CA3&& a3, CA4&& a4)
    {
      auto strong = lock_pointer (weak);
      return (get_pointer (strong) ->* method) (
        strong,
        std::forward <CA0> (a0),
        std::forward <CA1> (a1),
        std::forward <CA2> (a2),
        std::forward <CA3> (a3),
        std::forward <CA4> (a4)
      );
    }

  };

  template <typename Strong, typename Result, typename Target>
  AsyncMethod0 <Strong, Result, Target> async_method (const Strong& strong, Result (Target::*method) (Strong&))
  {
    return AsyncMethod0 <Strong, Result, Target> (strong, method);
  }

  template <typename Strong, typename Result, typename Target, typename A0>
  AsyncMethod1 <Strong, Result, Target, A0> async_method (const Strong& strong, Result (Target::*method) (Strong&, A0))
  {
    return AsyncMethod1 <Strong, Result, Target, A0> (strong, method);
  }

  template <typename Strong, typename Result, typename Target, typename A0, typename A1>
  AsyncMethod2 <Strong, Result, Target, A0, A1> async_method (const Strong& strong, Result (Target::*method) (Strong&, A0, A1))
  {
    return AsyncMethod2 <Strong, Result, Target, A0, A1> (strong, method);
  }

  template <typename Strong, typename Result, typename Target, typename A0, typename A1, typename A2>
  AsyncMethod3 <Strong, Result, Target, A0, A1, A2> async_method (const Strong& strong, Result (Target::*method) (Strong&, A0, A1, A2))
  {
    return AsyncMethod3 <Strong, Result, Target, A0, A1, A2> (strong, method);
  }

  template <typename Strong, typename Result, typename Target, typename A0, typename A1, typename A2, typename A3>
  AsyncMethod4 <Strong, Result, Target, A0, A1, A2, A3> async_method (const Strong& strong, Result (Target::*method) (Strong&, A0, A1, A2, A3))
  {
    return AsyncMethod4 <Strong, Result, Target, A0, A1, A2, A3> (strong, method);
  }

  template <typename Strong, typename Result, typename Target, typename A0, typename A1, typename A2, typename A3, typename A4>
  AsyncMethod5 <Strong, Result, Target, A0, A1, A2, A3, A4> async_method (const Strong& strong, Result (Target::*method) (Strong&, A0, A1, A2, A3, A4))
  {
    return AsyncMethod5 <Strong, Result, Target, A0, A1, A2, A3, A4> (strong, method);
  }

#else // MSVC 2013+ or other compiler
  
  template <typename Strong, typename Result, typename Target, typename... Args>
  class AsyncMethod
  {
    typedef typename WeakEquiv <Strong>::Type Weak;

  public:
    typedef Result (Target::*Method) (Strong&, Args...);

  private:
    Weak   weak;
    Method method;

  public:
    AsyncMethod (const Strong& strong, Method method) :
      weak   (unlock_pointer (strong)),
      method (method)
    { }
    
    template <typename... ArgVals>
    Result operator () (ArgVals&&... vals)
    {
      auto strong = lock_pointer (weak);
      return (get_pointer (strong) ->* method) (strong, std::forward <ArgVals...> (vals...);
    }

  };
  
  template <typename Strong, typename Result, typename Target, typename... Args>
  AsyncMethod <Strong, Result, Target, Args...> async_method (const Strong& strong, Result (Target::*method) (Strong&, Args...))
  {
    return AsyncMethod <Strong, Result, Target, Args...> (strong, method);
  }

#endif
  
} // namespace Rk
