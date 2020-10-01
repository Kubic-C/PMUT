#include "Headers/xMain.h"

#define null 0

int main()
{	
	WSAData WinsockData;
	Start(&WinsockData); // Start up all librarys as well as PMUT
	PMUT();             // Main loop
	End();             // Clean & Close all librarys
	return 0;
}