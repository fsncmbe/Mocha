#include <utils.hpp>

namespace mocha
{

void log(int level, const char *text)
{
  switch (level)
  {
    case LogLevel::DEBUG: std::cout << "DEBUG: " << text << "\n"; break;
    case LogLevel::INFO:  std::cout << "INFO: " << text << "\n"; break;
    case LogLevel::WARNING: std::cout << "WARNING: " << text << "\n"; break;
    case LogLevel::ERROR: std::cout << "ERROR: " << text << "\n"; break;
    case LogLevel::FATAL: std::cout << "FATAL: " << text << "\n"; break;
    default: break;
  }
}

} // mocha