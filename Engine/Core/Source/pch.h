#pragma once

#ifdef MECHA_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif

#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <random>
#include <filesystem>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <sstream>
#include <array>
#include <vector>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdint.h>
#include <cstdint>
#include <cstdlib>
#include <minmax.h>

#ifdef MECHA_PLATFORM_WINDOWS
	#include <Windows.h>
#endif