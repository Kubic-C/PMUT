#pragma once
#ifndef HIGH_LEVEL_NETWORKING_H
#define HIGH_LEVEL_NETWORKING_H

#include "Sockets.h"
#include "Servers.h"

#define FAILED_NETWORK_STARTUP -1
#define SUCCESS_ON_NETWORK_STARTUP 0

namespace HLnet
{
	int NetworkStartup(WSAData* Data);

	void NetworkCleanup();
}

#endif HIGH_LEVEL_NETWORKING_H