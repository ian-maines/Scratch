// Copyright Ian Maines
// 04/01/2016
// Windows Socket Class.
#pragma once

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <vector>

#include <iostream>
#include <string>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

namespace socket_helpers
{
inline in_addr string_to_ip (std::wstring strWIp)
	{
	in_addr ip;
	std::string strIp (strWIp.begin (), strWIp.end ());
	if (!inet_pton (AF_INET, strIp.c_str (), &ip))
		{
		TRACE ("Invalid IP Address: %s", strIp);
		std::cout << "Invalid IP address: " << strIp << "\n";
		throw;
		}
	TRACE ("IP address '%s' converted to '%X'", strIp.c_str (), ip.S_un.S_addr);
	return ip;
	}
}

namespace libsocket
{
class socket
	{
	public:
		socket ();

		bool connect (in_addr ip, int32_t port, timeval timeout);
		void disconnect ();
		bool connected (timeval timeout);	// Specify time to wait to determine if connected.
		bool send (const std::vector<char>& data);
		std::vector<char> recieve ();

	private:
		SOCKET m_socket;
		bool m_connected;
	};
}