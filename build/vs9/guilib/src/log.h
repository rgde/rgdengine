#pragma once

#include <boost/function.hpp>

namespace gui
{
	enum LogLevel
	{
		LogSystem,
		LogMessage,
		LogWarning,
		LogError,
		LogCritical
	};

	typedef boost::function<void (LogLevel, const std::string&) > LoggerCallback;
}