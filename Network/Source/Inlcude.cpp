#include "NHeaders/Include.h"

namespace HLnet
{
	int NetworkStartup(WSAData* Data)
	{
		int Result = WSAStartup(MAKEWORD(2, 2), Data);
		if (Result == FAILED_NETWORK_STARTUP)
			return FAILED_NETWORK_STARTUP;
		// Network Startup initially good
		if (HIBYTE(Data->wVersion) != 2 || LOBYTE(Data->wVersion) != 2)
			return FAILED_NETWORK_STARTUP;
		// Network startup is successful at getting the right version and is usable

		return SUCCESS_ON_NETWORK_STARTUP;
	}

	void NetworkCleanup()
	{
		WSACleanup();
	}
}