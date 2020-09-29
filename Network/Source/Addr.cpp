#include "NHeaders\Addr.h"

namespace HLnet
{
	IPVersion GetTypeFromString(const std::string& Str)
	{
		for (const char& Char : Str)
		{
			switch (Char)
			{
			case ':':
				return IPVersion::_6;

			case '.':
				return IPVersion::_4;

			default:
				break; // continue
			}
		}

		return IPVersion::UNKNOWN;
	}

	std::string IPV4ntop(in_addr Addr)
	{
		char Buffer[16];
		inet_ntop(AF_INET, (void*)&Addr, Buffer, 16);
		return std::string(Buffer);
	}

	in_addr IPV4pton(const std::string& Str)
	{
		in_addr Addr;
		inet_pton(AF_INET, Str.c_str(), (void*)&Addr);
		return Addr;
	}

	std::string IPV6ntop(in_addr6 Addr)
	{
		char Buffer[40];
		inet_ntop(AF_INET6, (void*)&Addr, Buffer, 40);
		return std::string(Buffer);
	}

	in_addr6 IPV6pton(const std::string& Str)
	{
		in_addr6 Addr;
		inet_pton(AF_INET6, Str.c_str(), (void*)&Addr);
		return Addr;
	}

	sockaddr_in MakeAddr(const std::string& Ip, const int& Port)
	{
		sockaddr_in Addr;
		Addr.sin_addr = IPV4pton(Ip);
		Addr.sin_port = htons(Port);
		Addr.sin_family = AF_INET;
		return Addr;
	}

	sockaddr_in6 MakeAddr6(const std::string& Ip, const int& Port)
	{
		sockaddr_in6 Addr;
		Addr.sin6_addr = IPV6pton(Ip);
		Addr.sin6_port = htons(Port);
		Addr.sin6_family = AF_INET6;
		return Addr;
	}
}
