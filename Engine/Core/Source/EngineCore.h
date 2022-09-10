#pragma once

// Platform detection using predefined macros
#ifdef _WIN32
/* Windows x64/x86 */
#ifdef _WIN64
/* Windows x64  */
#define RETRO_PLATFORM_WINDOWS
#else
/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#else
/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

namespace Retro
{
    namespace Core
    {
        template <typename T>
        using Scope = std::unique_ptr<T>;

        template <typename T, typename... Args>
        constexpr Scope<T> CreateScope(Args &&...args)
        {
            return std::make_unique<T>(std::forward<Args>(args)...);
        }

        template <typename T>
        using Ref = std::shared_ptr<T>;

        template <typename T, typename... Args>
        constexpr Ref<T> CreateRef(Args &&...args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }
    }
}