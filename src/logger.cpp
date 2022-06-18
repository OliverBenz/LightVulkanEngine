#include "logger.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>


logMessage(LogLevel level, const char* message) {
	std::string levelMessage;
	switch(level) {
		case LogLevel::Info:
			levelMessage = "[Info]";
			break;
		case LogLevel::Debug:
			levelMessage = "[Debug]";
			break;
		case LogLevel::Error:
			levelMessage = "[Error]";
			break;
		case LogLevel::Critical:
			levelMessage = "[Critical]";
			break;
	}

	auto timeStruct = std::time(nullptr);
	auto time = *std::localtime(&timeStruct);

	std::cerr << std::put_time(&tm, "[%d-%m-%Y %H-%M-%S]") << "  " << levelMessage << "  " << message;
}