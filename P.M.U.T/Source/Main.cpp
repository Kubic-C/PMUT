#pragma once
#include "Headers/xMain.h"

// For winsock
WSAData WinsockData;

int main()
{	
	Start(&WinsockData); // Start up all librarys as well as PMUT
	PMUT();             // Main loop
	End();             // Clean & Close all librarys
	return 0;
}