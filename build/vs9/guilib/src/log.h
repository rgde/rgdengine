#pragma once

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