
#ifndef DEBUG_HPP
#define DEBUG_HPP

class Debug
{
 public:
  static void Log(const char* message, ...);
  static void Warning(const char* message, ...);
  static void Error(const char* message, ...);

 private:
  Debug() = delete;
  Debug(const Debug& other) = delete;
  Debug(Debug&& other) = delete;
  Debug& operator=(const Debug& other) = delete;
  Debug& operator=(Debug&& other) = delete;
  ~Debug() = delete;
};

#endif // DEBUG_HPP
