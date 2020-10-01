#pragma once
#ifndef USER_H
#define USER_H

#include "pConstants.h"
#include "Error.h"

// Handling the user input from console
struct Input
{
	std::vector<std::string> Arguments;
	std::string Command;
};

namespace ConsoleParser 
{
	static Input Parse(std::string& Source)
	{
		Input Final;

		// Parsing for command
		for (char& Char : Source) // Get the command
		{
			if (Char != ' ')
				Final.Command += Char;
			else
				goto TYPE_PARSING;
		}

	TYPE_PARSING:
		// Parsing for Type
		size_t TIndex = Source.find(TYPE_QUALIFIER);
		if (TIndex == std::string::npos)
			goto ARGUMENT_PARSING;
		// -t or type qualifier exist
		if (TIndex + 3 >= Source.size())
			throw TheInputGivenWasNotBigEnough[TIndex];

		Final.Command = SUB_FUNC_TYPE + Final.Command + "-t=";
		for (size_t I = TIndex + 3; I < Source.size(); I++)
		{
			if (Source[I] != ' ')
				Final.Command += Source[I];
			else
				goto ARGUMENT_PARSING;
		}


	ARGUMENT_PARSING:
		size_t ArgumentStart = Source.find(ARG_STATMENT);
		size_t ArgumentEnd = Source.find(ARG_END);
		if (ArgumentStart == std::string::npos)
			return Final; // skip the rest of argument parsing
		else if (ArgumentEnd == std::string::npos)
			throw CouldNotFindStr[ARG_END]; // Argument statment exists but argument end does not exist
		if (ArgumentEnd < ArgumentStart)
			throw EndingBeforeStart[ARG_END];
		// Arguments are good to be parsed
		std::string Argument;
		ArgumentStart += 3;
		for (/* not needed*/; ArgumentStart < ArgumentEnd; ArgumentStart++)
		{
			if (Source[ArgumentStart] == ARG_SEPERATOR)
			{
				Final.Arguments.push_back(Argument); // Push back argument
				Argument.clear();
			}
			else
			{
				Argument += Source[ArgumentStart];
			}
		}

		Final.Arguments.push_back(Argument); // To get the last argument

		return Final;
	}

	static void GetRidOfSpaces(Input& Input_)
	{
		for (std::string& Str : Input_.Arguments)
		{
			Str.erase(std::remove(Str.begin(), Str.end(), ' '), Str.end());
		}
	}
}

namespace
{
	std::istream& GetLine(std::string& Input_)
	{
		std::cout << "[PMUT]";
		return std::getline(std::cin, Input_);
	}

	template<typename ... Params>
	void Print(const Params& ... Args)
	{
		((std::cout << Args), ... );
	}
}


#endif // USER_H