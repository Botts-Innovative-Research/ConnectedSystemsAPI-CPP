// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.
// Every .cpp file must include "pch.h" before anything else; MSVC skips any includes that come before it.

#ifndef PCH_H
#define PCH_H

// Standard library
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <variant>
#include <vector>

// Third-party headers used by CSAPI. They are large and rarely change.
#include <nlohmann/json.hpp>
#include <httplib.h>

#include "CppUnitTest.h"

// CSAPI itself. It changes often, but every test file includes it anyway, so a change to it recompiles
// every file either way; precompiling it makes edits to the tests themselves much faster.
#include "ConnectedSystemsAPI.h"

#endif //PCH_H
