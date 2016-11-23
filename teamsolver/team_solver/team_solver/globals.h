#pragma once
#include <iostream>
#include <string>

constexpr size_t salary_cap (50000);

enum class log_level
	{
	minimal = 0,
	verbose
	};

namespace
{
log_level global_level = log_level::minimal;
void write (const std::string& line)
	{
	std::cout << line;
	}
}

inline void SetLogLevel (log_level level)
	{
	global_level = level;
	}

struct verbose;
struct minimal;

template <typename level>
struct debug_logger
	{
	static void log (const std::string& line);
	};

inline void debug_logger<verbose>::log (const std::string& line)
	{
	if (global_level == log_level::verbose)
		{
		write (line);
		}
	}

inline void debug_logger<minimal>::log (const std::string& line)
	{
	if (global_level == log_level::minimal || global_level == log_level::verbose)
		{
		write (line);
		}
	}