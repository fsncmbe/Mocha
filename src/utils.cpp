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

void log(int level, int i)
{
  std::string s = std::to_string(i);
  log(level, s.c_str());
}

void log(int level, double d)
{
  std::string s = std::to_string(d);
  log(level, s.c_str());
}

void log(int level, float f)
{
  std::string s = std::to_string(f);
  log(level, s.c_str());
}

} // mocha