// Copyright 2019 Ian Maines

#include "pch.h"
#include "..\card.h"

#include "..\..\gtest\gtest.h"

namespace
	{
	TEST (CCard, Constructor)
		{
		CCard c1 (value_t::Ace, suit_t::Spades);

		EXPECT_STREQ (c1.print ().c_str(), "AS");
		}
	}