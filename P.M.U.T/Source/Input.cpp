#include "Headers/Input.h"

Input::Input()
	: Source(""), Command(""), Type(""), Args(std::vector<std::string>())
{
}

Input::Input(const std::string& Source_)
	: Source(Source_), Command(""), Type(""), Args(std::vector<std::string>())
{
}

std::string Input::GetSafe(std::vector<std::string> VStr, const size_t& Index)
{
	bool IndexMoreThenSize = Index <= VStr.size();
	return IndexMoreThenSize ?
		VStr[Index] : throw InvalidArgs("Index : " + std::to_string(Index));
}

std::vector<std::string> Input::_Split(
	const std::string& Start,
	const std::string& Delim,
	const int& EndAmount) const
{
	size_t Index = Start == "" ? 0 : Source.find(Start);
	if (Index == std::string::npos || !SourceGood())
	{
		throw CouldNotFindStr(Start);
	}

	std::vector<std::string> SplitStr;
	std::string Word;
	int Seen = 0;

	for (size_t I = Index; I < Source.size(); I++)
	{
		if (Delim.find(Source[I]) != std::string::npos)
		{
			Seen++;
			SplitStr.push_back(Word);
			Word.clear();
			// Check to see if we have seen one of the end characters enough
			if (Seen == EndAmount)
				return SplitStr;
		}
		else
			Word += Source[I];
	}

	if (Word.size() > 0)
		SplitStr.push_back(Word);

	return SplitStr;
}

void Input::SetType()
{
	this->Type = GetSafe(_Split(TYPE_QUALIFIER, " ", 2), 1);
}

void Input::SetCommand()
{
	//							  vvv get first string
	this->Command = GetSafe(_Split("", " ", 1), 0);
	this->CommandCopy = this->Command;

	try
	{
		SetType();
		Command = (SUB_FUNC_TYPE + Command + "T" + Type);
	} // No Type qualifier present
	catch (Error& X)
	{
		return;
	}
}

void Input::SetArgs(bool NoSpaces)
{
	std::string SourceArgs = "";
	size_t Index = TermArgsGood();
	CopyArgsFromTerm(SourceArgs, std::move(Index));
	_SetArgsImpl(SourceArgs, std::move(NoSpaces));
}

void Input::SetAll()
{
	SetType();
	SetCommand();
	SetArgs();
}

void Input::Clear()
{
	//this->ArgStream.clear();
	this->Args.clear();
	this->Source.clear();
	this->Type.clear();
	this->Command.clear();
}

bool Input::SourceGood() const
{
	bool Good = 
		Source.find_first_not_of(" ") != std::string::npos ||
		Source.find_first_not_of("") != std::string::npos;

	return Good;
}

inline size_t Input::TermArgsGood()
{
	/* IDEA : Possibly add boolean values like so :
	 int ArgGood = IndexOfStart == std::string::npos, and so on so forth */

	size_t&& IndexOfArgStart = Source.find(ARG_STATMENT);
	if (IndexOfArgStart == std::string::npos) // Check to see if the arg keyword/statment is present
		throw CouldNotFindStr(ARG_STATMENT);
	// arg statment is present; continue

	size_t&& IndexOfArgEnd = Source.find(ARG_END);
	if (IndexOfArgEnd == std::string::npos) // Check to see if the arg ending token is there
		throw CouldNotFindStr(std::string() = ARG_END);
	// arg ending token is there; continue

	// make sure the arg ending token is not before the arg start keyword/statment
	if (IndexOfArgEnd <= IndexOfArgStart)
		throw EndingBeforeStart(std::string() = ARG_END);

	return IndexOfArgStart;
}

inline void Input::CopyArgsFromTerm(std::string& Str, size_t&& Index)
{
	for (size_t I = Index + 3; I < Source.size(); I++) // Make substring of Source and put into SourceArg
	{
		switch (Source[I])
		{
		case ARG_END:
			Str += Source[I];
			return; // exit out of both the switch and the for statement

		default:
			Str += Source[I];
		}
	}
}

inline void Input::_SetArgsImpl(std::string& Str, bool&& NoSpaces)
{
	// Meant for a temparory holding of a single argument
	std::string Arg = "";
	for (size_t I = 0; I < Str.size(); I++)
	{
		if (Str[I] == ARG_SEPERATOR || Str[I] == ARG_END)
		{
			// Remove white space(saves space) if NoSpaces is True
			if (NoSpaces)
				Arg.erase(std::remove(Arg.begin(), Arg.end(), ' '), Arg.end());

			// ArgStream << Arg;
			Args.push_back(Arg);
			Arg.clear(); // Clear Arg Str
		}
		else // Add char from Str
			Arg += Str[I]; 

	}
}

std::string Input::operator[](const int& Index)
{
	return GetSafe(Args, Index);
}
