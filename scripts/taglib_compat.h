
#pragma once

#include <taglib/taglib.h>

// Determine if we are building against TagLib 2.x or later.
// Some TagLib distributions define TAGLIB_VERSION, others define
// TAGLIB_MAJOR_VERSION, so we support both here.
#ifndef TAGLIB_IS_V2_OR_LATER
#if (defined(TAGLIB_VERSION) && (TAGLIB_VERSION >= 0x020000)) || \
    (defined(TAGLIB_MAJOR_VERSION) && (TAGLIB_MAJOR_VERSION >= 2))
#define TAGLIB_IS_V2_OR_LATER 1
#endif
#endif

#if defined(TAGLIB_IS_V2_OR_LATER)
namespace TagLib {
  using uint = unsigned int;
}
#endif

// TagLib 2.x: createFrame signature changed.
// Old code uses createFrame(ByteVector, TagLib::uint)
// New API expects createFrame(ByteVector, const Header*)
#if defined(TAGLIB_IS_V2_OR_LATER)
  #ifndef GST_TAGLIB_CREATEFRAME_COMPAT
    #define GST_TAGLIB_CREATEFRAME_COMPAT 1
    #ifdef createFrame
      #undef createFrame
    #endif
    #define createFrame(DATA, VERSION) \
      createFrame((DATA), (const TagLib::ID3v2::Header*)nullptr)
  #endif
#endif
