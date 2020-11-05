#pragma once

#include <iostream>
#include <sstream>

enum LogLevel {
	ERROR,
	WARNING,
	INFO,
	DEBUG
};

class Logger {
public:
	Logger(LogLevel level = ERROR) {
		m_level = level;
	}

	template <typename T>
	Logger& operator<<(T const& value) {
		m_buffer << value;
		return *this;
	}

	~Logger() {
		m_buffer << std::endl;
		std::cerr << m_buffer.str();
	}
private:
	LogLevel m_level;
	std::ostringstream m_buffer;
};

#define LOG(level) \
if (level > LogLevel::level) ; \
else Logger(level)