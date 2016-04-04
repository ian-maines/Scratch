#pragma once

#include "stdafx.h"

#include <string>
#include "debug.h"

namespace libsocket
{
inline std::wstring get_system_error (int error)
	{
	constexpr int strSize = 128;
	TCHAR s[strSize];
	FormatMessageW (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, error,
					MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
					s, strSize, NULL);
	std::wstring strVal (s);
	return strVal;
	}

// ********************************************************
inline void process_socket_error (int wsa_last_err, std::string func)
	{
	std::wstring strWErr = get_system_error (wsa_last_err);
	std::string strErr (strWErr.begin (), strWErr.end ());
	std::cout << "Socket error '" << strErr << "' (" << wsa_last_err << ") in '" << func << "'\n";
	TRACE ("Socket error '%d' in '%s'", wsa_last_err, func);
	}
}