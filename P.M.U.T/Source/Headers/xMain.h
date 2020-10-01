/* --- XMAIN_H, xMain.h --- 
 * Main loop for PMUT app
*/

#pragma once
#ifndef XMAIN_H
#define XMAIN_H

#include "Command.h"
#include <tchar.h>

// MISSING INCLUDE; NOTIFY
#ifndef XCOMMAND_H
#error Include or import: xCommand.h, or/and Command.h
#endif // XCOMMAND_H

#ifndef XERROR_H
#error Include or import: xError.h, or/and Error.h
#endif // XERROR_H

namespace
{
	HANDLE OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	inline void PMUT()
	{
		static std::string InputTerm;
		while (GetLine(InputTerm, OutputHandle))
		{
			try
			{
				// Make sure the source(user's input) is not just a bunch of spaces or nothing
				if (InputTerm.find_first_not_of(' ') == std::string::npos)
					continue;

				// Call it
				SetConsoleTextAttribute(OutputHandle, FOREGROUND_GREEN);
				Input Input_ = ConsoleParser::Parse(InputTerm);
				Caller::CallCommand(Input_);

			}// PMUT Errror vvv
			catch (Error& x)
			{
				PrintErr(OutputHandle, x.zError, " < ", x.Cause, " >\n");
			}// all other errors go below
			catch (std::invalid_argument& x)
			{
				PrintErr(OutputHandle, x.what(), '\n');
			}
			catch (std::bad_alloc& x)
			{
				PrintErr(OutputHandle, x.what(), '\n');
			}
		}
	}


	void End()
	{
		HLnet::NetworkCleanup();
	}

	void Start(WSAData*&& Data)
	{
		// vvv Info About PMUT vvv
		SetConsoleTextAttribute(OutputHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		Print(PMUT_, '\n');
		Print("Type 'info' to get some info about PMUT, & how to report bugs\n");
		SetConsoleTextAttribute(OutputHandle, 15);

		SetConsoleTitle(_T(std::string(PMUT_).c_str()));
		AddCommand(CInitCmd, NO_ARGS, MakeSub("restart", "commands")); // Only exception when it comes to adding commands
		CommandStart();
		int Good = HLnet::NetworkStartup(std::forward<WSAData*>(Data));
		if (Good != 0)
		{
			Print("Failed to start Winsock2\n");
			End();
		}
	}
}


#endif