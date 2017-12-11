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
    ordinary = 0,
    lead_surrogate,
    trail_surrogate,
    noncharacter,
    bad
  };

  static constexpr CodepointSemantic codepoint_semantic (char32 cp) {
    if (cp > 0x10ffff)
      return CodepointSemantic::bad;
    else if (cp >= 0xd800 && cp <= 0xdbff)
      return CodepointSemantic::lead_surrogate;
    else if (cp >= 0xdc00 && cp <= 0xdfff)
      return CodepointSemantic::trail_surrogate;
    else if (is_codepoint_noncharacter (cp))
      return CodepointSemantic::noncharacter;
    else
      return CodepointSemantic::ordinary;
  }

  static constexpr bool is_codepoint_ordinary (char32 cp) {
    return codepoint_semantic (cp) == CodepointSemantic::ordinary;
  }
}

