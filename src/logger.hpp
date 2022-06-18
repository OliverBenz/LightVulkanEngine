#pragma once

enum class LogLevel {
	Info,
	Debug,
	Error,
	Critical
};

logMessage(LogLevel level, const char* message);