#pragma once

#include "PlatformDetection.h"

#include <memory>

#ifdef RETRO_DEBUG
#if defined(RETRO_PLATFORM_WINDOWS)
#define RETRO_DEBUGBREAK() __debugbreak()
#elif defined(RETRO_PLATFORM_LINUX)
#include <signal.h>
#define RETRO_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define RETRO_ENABLE_ASSERTS
#else
#define RETRO_DEBUGBREAK()
#endif

#define RETRO_EXPAND_MACRO(x) x
#define RETRO_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define RETRO_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Retro
{
	template <typename T>
	using Unique = std::unique_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Unique<T> CreateUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Shared = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Shared<T> CreateShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Logger/Logger.h"
#include "Core/Assert.h"
