#include "Headers/xCommand.h"

Command::Command(CmdType Function_, const int& Args_, const bool& NoEditToArgs_)
	: Function(Function_), NeededArgs(Args_), NoEditToArgs(NoEditToArgs_)
{
}

Command::~Command()
{
}

void Command::operator()(std::vector<std::string>& Vec)
{
	Function(Vec);
}

int Command::GetNeededArgs() const
{
	return NeededArgs;
}

bool Command::GetNoEditToArgs() const
{
	return NoEditToArgs;
}
