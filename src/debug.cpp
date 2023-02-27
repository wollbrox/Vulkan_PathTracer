
#include <VulkanPT/debug.hpp>
#include <cstdarg>
#include <string>

static void WriteLog(const char* prepend, const char* message, va_list args)
{ vprintf((std::string(prepend) + message + "\n").c_str(), args); }

void Debug::Log(const char* message, ...)
{
#ifdef DEBUG
  va_list args;
  va_start(args, message);
  WriteLog("\033[0;32m[LOG]: \033[0m", message, args);
  va_end(args);
#endif // DEBUG
}

void Debug::Warning(const char* message, ...)
{
#ifdef DEBUG
  va_list args;
  va_start(args, message);
  WriteLog("\033[0;33m[WARNING]: \033[0m", message, args);
  va_end(args);
#endif // DEBUG
}

void Debug::Error(const char* message, ...)
{
#ifdef DEBUG
  va_list args;
  va_start(args, message);
  WriteLog("\033[0;31m[ERROR]: \033[0m", message, args);
  va_end(args);
#endif // DEBUG
}
