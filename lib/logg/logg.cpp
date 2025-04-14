#include <stdio.h>
#include <stdarg.h>

#include "logg.hpp"

Level Logg::level_ = Level::DEBUG;

void Logg::setLevel(Level level)
{
	Logg::level_ = level;
}

void Logg::logMessage(Level level, const char *tag, const char *message, ...)
{
	if (level > Logg::level_)
		return;

	printf("[%s][%s] ", getLevelString(level), tag);

	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);

	printf("\n");
}

const char *Logg::getLevelString(Level level)
{
	switch (level)
	{
	case DEBUG:
		return "DEBUG";
	case INFO:
		return "INFO";
	case WARN:
		return "WARN";
	case ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

void Logg::debug(const char *tag, const char *message, ...)
{
	logMessage(Level::DEBUG, tag, message);
}

void Logg::info(const char *tag, const char *message, ...)
{
	logMessage(Level::INFO, tag, message);
}

void Logg::warn(const char *tag, const char *message, ...)
{
	logMessage(Level::WARN, tag, message);
}

void Logg::error(const char *tag, const char *message, ...)
{
	logMessage(Level::ERROR, tag, message);
}