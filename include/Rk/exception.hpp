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

#include <Rk/types.hpp>

#include <system_error>
#include <stdexcept>

namespace Rk {
  namespace detail {
#ifdef _WINDOWS
    extern "C" __declspec(dllimport) u32 __stdcall GetLastError ();
#endif

    static inline int last_error () {
#ifdef _WINDOWS
      return (int) GetLastError ();
#else
      return errno;
#endif
    }
  }

  static inline auto os_error (char const* message, int code = detail::last_error ()) {
    return std::system_error (code, std::system_category (), message);
  }
}

