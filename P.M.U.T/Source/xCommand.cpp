#include "Headers/xCommand.h"

Command::Command(CmdType Function_, const size_t& Args_, const bool& NoEditToArgs_)
	: Function(Function_), LowArgValue(Args_), HighArgValue(Args_), NoEditToArgs(NoEditToArgs_)
{
}

Command::Command(CmdType Function_, const size_t& LowArg_, const size_t& HighArg_, const bool& NoEditToArgs_)
	: Function(Function_), LowArgValue(LowArg_), HighArgValue(HighArg_), NoEditToArgs(NoEditToArgs_)
{
}

Command::~Command()
{
}

void Command::operator()(std::vector<std::string>& Vec)
{
	Function(Vec);
}

size_t Command::GetLowArgValue() const
{
	return LowArgValue;
}

size_t Command::GetHighArgValue() const
{
	return HighArgValue;
}

bool Command::GetNoEditToArgs() const
{
	return NoEditToArgs;
}
