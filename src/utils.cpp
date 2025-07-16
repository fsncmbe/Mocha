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

void log(int level, const std::string& text)
{
  log(level, text.c_str());
}

void log(int level, bool b)
{
  log(level, std::to_string(b).c_str());
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

void log(int level, glm::vec2 v2)
{
  std::string s = "x: " + std::to_string(v2.x) +  " y: " + std::to_string(v2.y);
  log(level, s.c_str());
}

void log(int level, glm::vec3 v3)
{
  std::string s = "x: "  + std::to_string(v3.x)
                + " y: " + std::to_string(v3.y)
                + " z: " + std::to_string(v3.z);
  log(level, s.c_str());
}

} // mocha