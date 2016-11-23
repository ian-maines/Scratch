// Copyright 2016 Ian Maines
#include "csv.h"
#include "globals.h"

#include <iostream>
#include <fstream>

namespace
	{
	std::string vec_to_string (const std::vector<std::string> val)
		{
		std::string out;
		for (auto elem : val)
			{
			out += elem;
			}
		return out;
		}

	// Remove enclosing '"' if they exist.
	void dequote (std::string& str)
		{
		if (str.size ())
			{
			if (*str.begin () == '"' && *(str.end () - 1) == '"')
				{
				str.erase (str.begin ());
				str.erase (str.end () - 1);
				}
			}
		}

	std::vector<std::string> tokenize (const std::string& str, const char token)
		{
		std::vector<std::string> out;
		std::string::const_iterator elem_begin = str.begin ();
		std::string::const_iterator elem_end = std::find (str.cbegin (), str.end (), token);

		while (elem_end != str.end ())
			{
			std::string current (elem_begin, elem_end);
			out.push_back (current);

			elem_begin = elem_end + 1; // Move past current token.
			elem_end = std::find (elem_begin, str.end (), token);
			}

		// Final element (may be the only element)
		std::string current (elem_begin, elem_end);
		out.push_back (current);

		return out;
		}
	}

namespace csv
{
bool CDkCsv::Import ()
	{
	std::ifstream file;
	file.open (m_filename, std::ios::in);

	if (!file.is_open ())
		{
		debug_logger<minimal>::log ("Unable to open file");
		return false;
		}
	
	// Nab everything from the file.
	std::vector<std::string> raw_lines;
	std::string raw_line;
	while (getline (file, raw_line))
		{
		raw_lines.push_back (raw_line);
		}

	// Convert each line into a vector of elements.
	lines_t final_lines;
	for (const auto& raw_line : raw_lines)
		{
		line_t current_line = tokenize (raw_line, ',');
		for (auto& elem : current_line)
			{
			dequote (elem);
			}
		final_lines.push_back (current_line);
		}

	m_data = final_lines;
	m_bImported = true;
	return true;
	}

lines_t CDkCsv::GetData ()
	{
	if (!m_bImported)
		{
		throw std::exception ();
		}
	return m_data;
	}


bool CRgCsv::Import ()
	{
	std::ifstream file;
	file.open (m_filename, std::ios::in);

	if (!file.is_open ())
		{
		debug_logger<minimal>::log ("Unable to open file");
		return false;
		}

	// Nab everything from the file.
	std::vector<std::string> raw_lines;
	std::string raw_line;
	while (getline (file, raw_line))
		{
		raw_lines.push_back (raw_line);
		}

	// Convert each line into a vector of elements.
	lines_t final_lines;
	for (const auto& raw_line : raw_lines)
		{
		line_t current_line = tokenize (raw_line, ',');
		for (auto& elem : current_line)
			{
			dequote (elem);
			}
		final_lines.push_back (current_line);
		}

	m_data = final_lines;
	m_bImported = true;
	return true;
	}

lines_t CRgCsv::GetData ()
	{
	if (!m_bImported)
		{
		throw std::exception ();
		}
	return m_data;
	}
}