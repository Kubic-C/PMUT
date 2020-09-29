#pragma once
#ifndef USER_H
#define USER_H

#include "pConstants.h"
#include "Error.h"

// Handling the user input from console
struct Input
{
	// Note: Ter OR Term is short for terminal

	// Source, String from terminal/console
	std::string Source; 

	// Type
	std::string Type;

	// Command
	std::string Command;
	std::string CommandCopy; // For errors 

	// Argument List
	std::vector<std::string> Args;

	// Function args
	std::vector<std::string> FuncArgs;

	// Arguments Stream
	// std::stringstream ArgStream;

	Input();

	Input(const std::string& Source_);

	// Get part of a vec safely
	std::string GetSafe(std::vector<std::string> VStr, const size_t& Index);

	// Splits source
	std::vector<std::string> _Split(
		const std::string& Start,
		const std::string& Delim,
		const int& EndAmount) const;

	// Sets member: Type
	void SetType();

	// Sets member: Command
	void SetCommand();

	// Sets member: Args(from console)
	void SetArgs(bool NoSpaces = true);

	// Sets all members
	void SetAll();

	// Clears All
	void Clear();

	// Checks if the user put nothing
	bool SourceGood() const;

	// returns true if : the -a[ and the ] are present in the console
	inline size_t TermArgsGood();

	// Copys the string arg from console to 
	inline void CopyArgsFromTerm(std::string& Str, size_t&& Index);

	// Set the arguments from Str
	inline void _SetArgsImpl(std::string& Str, bool&& NoSpaces = true);

	// Prepare FuncArgs
	inline void PrepareFuncArgs(const bool& NoEditToArgs = false, const bool& SetArgs_ = true)
	{
		if (SetArgs_)
		{
			SetArgs(NoEditToArgs);
			FuncArgs.reserve(Args.size());
			FuncArgs.insert(FuncArgs.end(), Args.begin(), Args.end());
		}
	}

	std::string operator[](const int& Index);
};

namespace
{
	std::istream& GetLine(Input& Input_)
	{
		std::cout << "[PMUT]";
		return std::getline(std::cin, Input_.Source);
	}

	template<typename ... Params>
	void Print(const Params& ... Args)
	{
		((std::cout << Args), ... );
	}
}


#endif // USER_H