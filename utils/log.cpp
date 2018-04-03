#include <iomanip>
#include <iostream>
#include <chrono>
#include "date.h"

enum log_levels {INFO, WARN, ERROR};

inline std::ostream& log(int log_level) {
  using namespace date;
  using namespace std::chrono;
  if (log_level == ERROR)
    return std::cerr << "\033[32m" << "[" << system_clock::now() << "] " << "\033[0m" << "\033[1;31m" <<   "Error:   "  << "\033[0m";
  else if (log_level == WARN)
    return std::cerr << "\033[32m" << "[" << system_clock::now() << "] " << "\033[0m" << "\033[1;33m" << "Warning: "  << "\033[0m";
  else
    return std::cout << "\033[32m" << "[" << system_clock::now() << "] " << "\033[0m" << "\033[1;32m" <<    "Info:    "  << "\033[0m";
}
