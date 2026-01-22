
#pragma once

#include <taglib/taglib.h>
#if defined(TAGLIB_VERSION) && (TAGLIB_VERSION >= 0x020000)
namespace TagLib {
  using uint = unsigned int;
}

// TagLib 2.x: createFrame signature changed.
// Old code uses createFrame(ByteVector, TagLib::uint)
// New API expects createFrame(ByteVector, const Header*)
#if defined(TAGLIB_MAJOR_VERSION) && (TAGLIB_MAJOR_VERSION >= 2)
  #ifndef GST_TAGLIB_CREATEFRAME_COMPAT
    #define GST_TAGLIB_CREATEFRAME_COMPAT 1
    #define createFrame(DATA, VERSION) \
      createFrame((DATA), (const TagLib::ID3v2::Header*)nullptr)
  #endif
#endif
