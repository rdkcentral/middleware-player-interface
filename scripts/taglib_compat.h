#pragma once
#include <taglib/taglib.h>

#if defined(TAGLIB_VERSION) && (TAGLIB_VERSION >= 0x020000)
namespace TagLib {
    using uint = unsigned int;
}
#endif
