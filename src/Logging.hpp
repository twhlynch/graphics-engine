#pragma once

#include <iostream>
#include <ostream>
#include <string>

#define FILENAME (std::string(__FILE__).substr(std::string(__FILE__).find_last_of("/\\") + 1))
#define LOG_FMT(message, type) "[" << type << "][" << FILENAME << "][Line " << __LINE__ << "] " << message << std::endl

#define LOG(message, type) std::cout << LOG_FMT(message, type)
#define ERR(message, type) std::cerr << LOG_FMT(message, type)

#define WARN(message) ERR(message, "WARN")
#define ERROR(message) ERR(message, "ERROR")
#define INFO(message) LOG(message, "INFO")
#define DEBUG(message) LOG(message, "DEBUG")