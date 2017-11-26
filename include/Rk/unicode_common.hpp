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

namespace Rk {
  enum class DecodeStatus {
    idle,
    pending,
    got_codepoint,
    bad_codepoint,
    got_surrogate,
    got_noncharacter,
    got_overlong,
    invalid_sequence
  };

  static constexpr bool is_codepoint_noncharacter (char32 cp) {
    return (cp >= 0xfdd0 && cp <= 0xfdef)
        || (cp & 0xfffe) == 0xfffe;
  }

  enum class CodepointSemantic {
    codepoint_ordinary = 0,
    codepoint_lead_surrogate,
    codepoint_trail_surrogate,
    codepoint_noncharacter,
    codepoint_bad
  };

  static constexpr CodepointSemantic codepoint_semantic (char32 cp) {
    if (cp > 0x10ffff)
      return codepoint_bad;
    else if (cp >= 0xd800 && cp <= 0xdbff)
      return codepoint_lead_surrogate;
    else if (cp >= 0xdc00 && cp <= 0xdfff)
      return codepoint_trail_surrogate;
    else if (is_codepoint_noncharacter (cp))
      return codepoint_noncharacter;
    else
      return codepoint_ordinary;
  }

  static constexpr bool is_codepoint_ordinary (char32 cp) {
    return codepoint_semantic (cp) == codepoint_ordinary;
  }
}

