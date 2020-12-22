#pragma once
#ifndef HL_NET_BASE_H
#define HL_NET_BASE_H

// ASIO 
#ifdef _WIN32
#pragma warning(push, 0)

#define _WIN32_WINNT 0x0A00

#define ASIO_STANDALONE
#include <asio.hpp>

#pragma warning(pop)
#endif // _WIN32

// OGLConsole
#include "OGLHeaders/Include.h"

namespace console
{
	namespace vars
	{
		GLOBAL console::console_master* pmut_console;
	}
}

namespace hlnet
{
	void get_html();
}

#endif HL_NET_BASE_H