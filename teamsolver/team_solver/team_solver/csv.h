// Copyright 2016 Ian Maines
#pragma once

#include <string>
#include <vector>

namespace csv
{
using line_t = std::vector<std::string>;
using lines_t = std::vector<line_t>;

class CDkCsv
	{
	public:
		CDkCsv (const std::string& filename)
			: m_bImported (false)
			, m_filename (filename)
			{}

		// Separate from ctor in case threading.
		bool Import ();
		lines_t GetData ();

	private:
		bool m_bImported;
		const std::string m_filename;
		lines_t m_data;
	};


class CRgCsv
	{
	public:
		CRgCsv (const std::string& filename)
			: m_bImported (false)
			, m_filename (filename)
			{}

		// Separate from ctor in case threading.
		bool Import ();
		lines_t GetData ();

	private:
		bool m_bImported;
		const std::string m_filename;
		lines_t m_data;
	};
}