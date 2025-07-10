#include <utils.hpp>

namespace mocha
{

void log(int level, const char *text)
{
  switch (level)
  {
    case LogLevel::DEBUG: std::cout << "DEBUG: " << text << "\n";
    case LogLevel::INFO:  std::cout << "INFO: " << text << "\n";
    case LogLevel::WARNING: std::cout << "WARNING: " << text << "\n";
    case LogLevel::ERROR: std::cout << "ERROR: " << text << "\n";
    case LogLevel::FATAL: std::cout << "FATAL: " << text << "\n";
    default: break;
  }
}

} // mocha