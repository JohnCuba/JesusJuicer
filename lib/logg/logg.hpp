#pragma once

enum Level
{
	ERROR,
	WARN,
	INFO,
	DEBUG,
};

class Logg
{
private:
	static Level level_;
	static void logMessage(Level level, const char *tag, const char *message, ...);

	static const char *getLevelString(Level level);

public:
	static void setLevel(Level level);
	static void debug(const char *tag, const char *message, ...);
	static void info(const char *tag, const char *message, ...);
	static void warn(const char *tag, const char *message, ...);
	static void error(const char *tag, const char *message, ...);
};
