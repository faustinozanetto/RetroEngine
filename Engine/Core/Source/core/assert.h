#pragma once

#include "core/base.h"
#include "logger/logger.h"
#include <filesystem>

#ifdef RETRO_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define RETRO_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
  {                                                       \
    if (!(check))                                         \
    {                                                     \
      RETRO##type##ERROR(msg, __VA_ARGS__);               \
      RETRO_DEBUGBREAK();                                 \
    }                                                     \
  }
#define RETRO_INTERNAL_ASSERT_WITH_MSG(type, check, ...) RETRO_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define RETRO_INTERNAL_ASSERT_NO_MSG(type, check) RETRO_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", RETRO_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define RETRO_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define RETRO_INTERNAL_ASSERT_GET_MACRO(...) RETRO_EXPAND_MACRO(RETRO_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RETRO_INTERNAL_ASSERT_WITH_MSG, RETRO_INTERNAL_ASSERT_NO_MSG))

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define RETRO_ASSERT(...) RETRO_EXPAND_MACRO(RETRO_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define RETRO_CORE_ASSERT(...) RETRO_EXPAND_MACRO(RETRO_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define RETRO_ASSERT(...)
#define RETRO_CORE_ASSERT(...)
#endif
