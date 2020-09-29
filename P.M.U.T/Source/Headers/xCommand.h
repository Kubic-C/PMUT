/* --- XCOMMAND_H, xCommand.h ---
 * Contains info about PMUT version and other meta-deta.
 * Command class meant to act like a wrapper over a function pointer
 *  
 * 
 * 
 * 
*/

#pragma once
#ifndef XCOMMAND_H
#define XCOMMAND_H

#define BUILD_NUMBER (std::string)"55"
#define PMUT_ "Porter Multi Use Terminal ( PMUT ) | Build[ " + BUILD_NUMBER + " ] "

#include "Input.h"
#include "NHeaders/Include.h"

#define CommandDecl(x) void x(std::vector<std::string>& Vector)
using namespace std::literals::chrono_literals;

template<typename T>
static T MakeZero()
{
	T Var;
	memset(&Var, 0, sizeof(T));
	return Var;
}

typedef void(*CmdType)(std::vector<std::string>&);
static std::vector<std::string> Empty = MakeZero<std::vector<std::string>>();

// Defines what command looks like
class Command
{
public:

	Command() = default;
	Command(CmdType Function_, const int& Args_, const bool& NoEditToArgs_ = false);
	~Command();

	void operator()(std::vector<std::string>& Vec = Empty);
	int GetNeededArgs() const;
	bool GetNoEditToArgs() const;

private:
	CmdType Function;
	int NeededArgs;
	bool NoEditToArgs;
};

#endif // XCOMMAND_H