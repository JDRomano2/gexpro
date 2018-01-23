#ifndef LOGGER_HPP
#define LOGGER_HPP

enum TLogLevel {
  logERROR,
  logWARNING,
  logINFO,
  logDEBUG,
  logDEBUG1,
  logDEBUG2,
  logDEBUG3,
  logDEBUG4
};

class Logger {
private:
  Logger(const Logger&);
  Logger& operator =(const Logger&);
  TLogLevel messageLevel;
protected:
  std::ostringstream os;
public:
  Logger();
  virtual ~Logger();
  std::ostringstream& get(TLogLevel level = logINFO);
  static TLogLevel& reportingLevel();
};

#endif
