// Winsock Learning.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Winsock Learning.h"

#include <iostream>
#include <string>
#include <vector>

#include "debug.h"
#include "mysocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
const std::string strSendData ("Testing 1,2,3, this is a test.");

// returns 0 on success
int init ()
	{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle (nullptr);

	if (hModule != nullptr)
		{
		// initialize MFC and print and error on failure
		if (!AfxWinInit (hModule, nullptr, ::GetCommandLine (), 0))
			{
			// TODO: change error code to suit your needs
			wprintf (L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
			}
		else
			{
			// TODO: code your application's behavior here.
			}
		}
	else
		{
		// TODO: change error code to suit your needs
		wprintf (L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
		}

	return nRetCode;
	}
}

// The one and only application object

CWinApp theApp;

using namespace std;

int main()
	{
	if (init ())
		{
		TRACE ("INIT FAILED");
		return -1;
		}

	libsocket::socket socket;
	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	if (socket.connect (socket_helpers::string_to_ip (L"192.168.0.3"), 32001, timeout))
		{
		std::vector<char> data (strSendData.begin (), strSendData.end ());
		if (socket.send (data))
			{
			data = socket.recieve ();
			// We're transmitting null-terminated strings, so we can filter out after the first null.
			std::string strRcvData (data.begin (), std::find (data.begin (), data.end (), '\0'));	
			std::cout << "Recieved data: " << strRcvData << "\n";
			}
		while (true)
			{
			string strin;
			std::getline (cin, strin);
			data.assign (strin.begin (), strin.end ());
			if (socket.send (data))
				{
				data = socket.recieve ();
				// We're transmitting null-terminated strings, so we can filter out after the first null.
				std::string strRcvData (data.begin (), std::find (data.begin (), data.end (), '\0'));
				std::cout << "Recieved data: " << strRcvData << "\n";
				}
			}
		}

	return 0;
	}
