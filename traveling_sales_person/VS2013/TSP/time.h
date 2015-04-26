//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     time class
// Date:            04/25/2015
//
//************************************************************
#ifndef TIME_H
#define TIME_H

#include <chrono>
#include <cstdint>

class time
	{
	public:
		time ()
			: m_ts (std::chrono::system_clock::now ().time_since_epoch ())
			, m_nSysClkPrdNum (std::chrono::system_clock::period::num)
			, m_nSysClkPrdDen (std::chrono::system_clock::period::den)
			, m_ts_ms (m_ts.count () * (m_nSysClkPrdNum) / (m_nSysClkPrdDen / time_ms))
			{ }

		void printsystimeparams ()
			{
			std::cout << "System Clock Numerator: " << m_nSysClkPrdNum << " System Clock Denominator: " << m_nSysClkPrdDen << std::endl;
			}

		// Returns time since the last epoch in econds
		unsigned long long int gets ()
			{
			return _gettime (time_sec);
			}

		// Returns time since the last epoch in ms
		unsigned long long int getms ()
			{
			return _gettime (time_ms);
			}

		// Returns time since the last epoch in us
		unsigned long long int getus ()
			{
			return _gettime (time_us);
			}

		// Returns time since the last epoch in ns	
		unsigned long long int getns ()
			{
			return _gettime (time_ns);
			}

		// Returns the elapsed milliseconds since this object was created.
		unsigned long long int elapsed_ms ()
			{
			return (std::chrono::system_clock::now ().time_since_epoch ().count () * (m_nSysClkPrdNum) / (m_nSysClkPrdDen / time_ms)) - _gettime (time_ms);
			}

		unsigned long long int elapsed_us ()
			{
			return (std::chrono::system_clock::now ().time_since_epoch ().count () * (m_nSysClkPrdNum) / (m_nSysClkPrdDen / time_us)) - _gettime (time_us);
			}



	private:
		// Returns the time since epoch in the specified time interval.
		std::intmax_t _gettime (std::intmax_t nTime)
			{
			if (nTime > m_nSysClkPrdDen)
				{
				return 1000 * _gettime (nTime / 1000);
				}
			else
				{
				return m_ts.count () * (m_nSysClkPrdNum) / (m_nSysClkPrdDen / nTime);
				}
			}

	private:
		// In finest grain available.
		std::chrono::system_clock::duration m_ts;
		// System clock period numerator and denominator
		std::intmax_t m_nSysClkPrdNum;
		std::intmax_t m_nSysClkPrdDen;
		// Time in ms since system time epoch
		unsigned long long int m_ts_ms;
	};

#endif // TIME_H