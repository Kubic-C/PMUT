// Parses files and will get specific parts of a file and copy the info to std::string
#pragma once
#ifndef SHADER_PARSER_H
#define SHADER_PARSER_H

#include "PCH\OGLPCH.h"
#include <fstream>

// When talking about the .exe\app's directory I write it as ExeDir

namespace OGL
{
	std::string GetExeDir();
	// Gets a line(s) of a file based on characrters
	inline std::string ParseForLines(std::string Dir, std::string Start)
	{
		std::ifstream Input; // Source/input from file
		Input.open(Dir);
		if (Input.bad() || !Input.is_open()) // make sure the std::ifstream is good, if not return ASCII \15(decimal)
		{
			return "Could not open file";
		}
	
		bool Read = false;
		std::string extracted_lines = "";
		std::string file_line = "";
		while (std::getline(Input, file_line)) // Keep going as long as there the eof(end of file) has not read
		{
			if (file_line.find("@") != std::string::npos) // the character '@' should be treated like a preprocceser in C\C++
			{
				if (file_line.find(Start + " end") != std::string::npos)
					return extracted_lines;

				else if (file_line.find(Start) != std::string::npos)
					Read = true;
			}
			else if (Read) // Read and copy the lines
			{
				// Add the file line to extracted lines and don't forget to 
				// add the newline character to replicate exactly what it looks like in the file
				extracted_lines += file_line + '\n';
			}
		}

		// Close file input and return the extracted lines
		Input.close();
		return extracted_lines;
	}
}

#endif