// Copyright Ian Maines
// 04/03/2016
// winsock initialization and cleanup object
#pragma once

#include"stdafx.h"
#include <atomic>

#include "helpers.h"

namespace details
{
// One-and-only-one WSA startup. Automatically called by the first socket created.
class winsock_initializer
	{
	public:
		static void get ();	// Call to init winsock comms.

	private:
		winsock_initializer ();
		~winsock_initializer ();

		// No copy or move.
		winsock_initializer (const winsock_initializer& rhs) = delete;
		winsock_initializer& operator= (const winsock_initializer& rhs) = delete;
		winsock_initializer (const winsock_initializer&& rhs) = delete;
		winsock_initializer& operator= (const winsock_initializer&& rhs) = delete;

	private:
		static winsock_initializer* m_pwsi;
	};
}
