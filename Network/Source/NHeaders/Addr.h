#pragma once
#ifndef IP_H
#define IP_H

#include "Constants.h"

namespace HLnet
{
	enum class IPVersion
	{
		UNKNOWN = -1,
		_4 = AF_INET,
		_6 = AF_INET6
	};

	// Determines IP Version/type(Ipv6, or ipv4) from a string
	extern IPVersion GetTypeFromString(const std::string& Str);

	// Convert from IPV4 Net to IPV4 Presentable
	extern std::string IPV4ntop(in_addr Addr);

	// Convert from IPV4 Presentable to IPV4 Net
	extern in_addr IPV4pton(const std::string& Str);

	// Convert from IPV6 Net to IPV6 Presentable
	extern std::string IPV6ntop(in_addr6 Addr);

	// Convert from IPV6 Presentable to IPV6 Net
	extern in_addr6 IPV6pton(const std::string& Str);

	// Makes an Addr with ipv4
	extern sockaddr_in MakeAddr(const std::string& Ip, const int& Port);

	// Makes an Addr with ipv6
	extern sockaddr_in6 MakeAddr6(const std::string& Ip, const int& Port);
}

#endif