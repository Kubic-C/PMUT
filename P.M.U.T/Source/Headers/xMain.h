/* --- XMAIN_H, xMain.h --- 
 * Main loop for PMUT app
*/

#pragma once
#ifndef XMAIN_H
#define XMAIN_H

#include "Command.h"

// MISSING INCLUDE; NOTIFY
#ifndef XCOMMAND_H
#error Include or import: xCommand.h, or/and Command.h
#endif // XCOMMAND_H

#ifndef XERROR_H
#error Include or import: xError.h, or/and Error.h
#endif // XERROR_H

namespace
{
	inline void PMUT()
	{
		static Input Input;
		while (GetLine(Input))
		{
			try
			{
				// Make sure the source(user's input) is not just a bunch of spaces or nothing
				if (!Input.SourceGood())
					continue;

				// Call it
				Caller::CallCommand(Input);

			}// PMUT Errror vvv
			catch (Error& x)
			{
				Print(x.zError, " < ", x.Cause, " >\n");
			}// all other errors go below
			catch (std::invalid_argument& x)
			{
				Print(x.what(), '\n');
			}
			catch (std::bad_alloc& x)
			{
				Print(x.what(), '\n');
			}

			Input.Clear(); // Clear input buffers
		}
	}

	void Start(WSAData*&& Data)
	{
		// vvv Info About PMUT vvv
		Print(PMUT_, '\n');

		AddCommand(CInitCmd, NO_ARGS, MakeSub("restart", "commands")); // Only exception when it comes to add commands
		CommandStart();
		HLnet::NetworkStartup(std::forward<WSAData*>(Data));
	}

	void End()
	{
		HLnet::NetworkCleanup();
	}
}


#endif