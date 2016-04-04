// Copyright Ian Maines
// 04/03/2016
// winsock initialization and cleanup object

#include"stdafx.h"

#include <iostream>
#include "winsock_initializer.h"

details::winsock_initializer* details::winsock_initializer::m_pwsi = nullptr;

namespace
{
// ****************************************************************************
void print_wsadata (int nResult, LPWSADATA WSAData)
	{
	std::cout << "WSAStartup results" << "\n"
		<< "WSAStartup return value: " << nResult << "\n"
		<< "WINSOCK Version: " << ((WSAData->wVersion & 0xFF00) >> 8) << "." << (WSAData->wVersion & 0x00FF) << "\n"
		<< "Max Supported Version: " << ((WSAData->wHighVersion & 0xFF00) >> 8) << "." << (WSAData->wHighVersion & 0x00FF) << "\n"
		<< "Description: " << WSAData->szDescription << "\n"
		<< "System Status: " << WSAData->szSystemStatus << "\n";
	}
}

namespace details
{
// ****************************************************************************
void winsock_initializer::get ()
	{
	if (!m_pwsi)
		{
		m_pwsi = new winsock_initializer;
		}
	}

// ****************************************************************************
winsock_initializer::winsock_initializer ()
	{
	WSADATA WSAData;
	int nResult = WSAStartup (MAKEWORD (2, 2), &WSAData);	// Request v2.2
	print_wsadata (nResult, &WSAData);
	}

// ****************************************************************************
winsock_initializer::~winsock_initializer ()
	{
	delete m_pwsi;
	if (WSACleanup () == SOCKET_ERROR)
		{
		//Uh-oh
		libsocket::process_socket_error (WSAGetLastError (), __func__);
		}
	}

}