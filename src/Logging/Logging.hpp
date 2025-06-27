#pragma once

#include <iostream>
#include <ostream>
#include <string>

#define DEBUG_ENABLED BUILD_DEBUG && 1
#define DEBUG_ERROR 1
#define DEBUG_WARN 1
#define DEBUG_INFO 1

#define FILENAME std::string(__FILE__).substr(std::string(__FILE__).find_last_of("/\\") + 1)
#define LOG_FMT(message, type) "[" << type << "][" << FILENAME << "][Line " << __LINE__ << "] " << message << "\n"

#define LOG(message, type) std::cout << LOG_FMT(message, type)
#define ERR(message, type) std::cerr << LOG_FMT(message, type)

#if DEBUG_ENABLED
#define DEBUG(message) LOG(message, "DEBUG")
#else
#define DEBUG(message)
#endif

#if DEBUG_WARN && DEBUG_ENABLED
#define WARN(message) ERR(message, "WARN")
#else
#define WARN(message)
#endif

#if DEBUG_ERROR && DEBUG_ENABLED
#define ERROR(message) ERR(message, "ERROR")
#else
#define ERROR(message)
#endif

#if DEBUG_INFO && DEBUG_ENABLED
#define INFO(message) LOG(message, "INFO")
#else
#define INFO(message)
#endif
