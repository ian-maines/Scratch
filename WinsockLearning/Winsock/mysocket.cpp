// Copyright Ian Maines
// 04/01/2016
// Windows Socket Class.

#include "stdafx.h"

#include "mysocket.h"
#include "debug.h"
#include "helpers.h"
#include "winsock_initializer.h"

namespace
{
// ********************************************************

// ********************************************************
sockaddr_in data_to_sockaddr (in_addr ip, int32_t port)
	{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip.S_un.S_addr;
	addr.sin_port = htons (port);
	return addr;
	}
} // END helpers

namespace libsocket	// Can't use namespace socket
{
socket::socket ()
	: m_connected (false)
	{
	// Call WSAStartup
	details::winsock_initializer::get ();

	m_socket = ::socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
		{
		int nCode (WSAGetLastError ());
		std::cout << "ERROR Creating socket: " << nCode << "\n";
		throw;
		}
	}

// ********************************************************
bool socket::connect (in_addr ip, int32_t port, timeval timeout)
	{
	bool bRetVal = false;
	// Make sure we're not connected.
	disconnect ();

	sockaddr_in addr (data_to_sockaddr (ip, port));
	int ret (::connect (m_socket, (sockaddr*)&addr, sizeof (addr)));

	if (ret == SOCKET_ERROR)
		{
		int last_err (WSAGetLastError ());
		process_socket_error (last_err, __func__);
		}

	bRetVal = connected (timeout);
	m_connected = bRetVal;
	return bRetVal;
	}

// ********************************************************
void socket::disconnect ()
	{
	if (m_connected)
		{
		if (closesocket (m_socket) == SOCKET_ERROR)
			{
			// There's note a lot we can do about that.
			process_socket_error (WSAGetLastError (), __func__);
			}
		m_connected = false;
		}
	}

// ********************************************************
bool socket::connected (timeval timeout)
	{
	bool bRetVal = false;
	// Check if we're connected
	fd_set write_fd;
	write_fd.fd_count = 1;
	write_fd.fd_array[0] = m_socket;
	const int select_retval (select (0, nullptr, &write_fd, nullptr, &timeout));
	if (select_retval == 0)
		{
		TRACE ("Timeout waiting for connect");
		std::cout << "Timeout waiting for connect\n";
		}
	else if (select_retval == SOCKET_ERROR)
		{
		process_socket_error (WSAGetLastError (), __func__);
		}
	else
		{
		bRetVal = true;
		}
	return bRetVal;
	}

// ********************************************************
bool socket::send (const std::vector<char>& data)
	{
	// FIXME: Don't exceed max size.
	// FIME: What if size sent is < requested?
	// FIXME: Are we connected?
	if (::send (m_socket, &data[0], data.size (), 0) == SOCKET_ERROR)
		{
		process_socket_error (WSAGetLastError (), __func__);
		return false;
		}
	return true;
	}

// ********************************************************
std::vector<char> socket::recieve ()
	{
	// FIXME: Are we connected?
	std::vector<char> rcv_buf (2048);	// FIXME we always get this many bytes in nBytes. Why?
	// Rather than receive way more stuff than we need at a time, go until we get nothing.
	int nBytes (::recv (m_socket, &rcv_buf[0], rcv_buf.capacity (), 0));
	if (nBytes == SOCKET_ERROR)
		{
		process_socket_error (WSAGetLastError (), __func__);
		}
	return rcv_buf;
	}
}