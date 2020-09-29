#pragma once
#ifndef SOCKETS_H
#define SOCKETS_H

#include "Error.h"
#include "Sets.h"
#include "Addr.h"
#include "CallingInfo.h"

namespace HLnet
{
	class BaseSocket
	{
	public:
		std::vector<Error> Errors;

		BaseSocket();
		BaseSocket(const SOCKET& Socket);

		// Zeros members, besides Errors
		inline void _ZeroMembers();

		// Makes a socket, and sets up Info4 and Info6
		virtual inline void SetSocket(const std::string& Ip, const int& Port);

		// Set SockAddr or SockAddr6(depends on param Ip)
		inline void SetSockAddr(const std::string& Ip, const int& Port);

		// Close the socket
		WinsockError Close();

		// Gets sockets
		SOCKET GetSocket();

		// Gets Info4
		sockaddr_in GetSockAddr();

		// Gets INnfo6
		sockaddr_in6 GetSockAddr6();

		// Gets the currentyl used type
		int& GetIPV();

		bool operator==(BaseSocket Other);

		bool operator!=(BaseSocket Other);

	protected:
		SOCKET Socket;  // Socket
		sockaddr_in Info4;  // Info about Socket
		sockaddr_in6 Info6; // Info about Socket6
		int Type; // Which sockaddr Socket(member) is using
	};

	class TCPSocket : public BaseSocket
	{
	public:

		TCPSocket();
		TCPSocket(const SOCKET& Socket_);
		TCPSocket(const std::string& Ip, const int& Port);
		TCPSocket(BaseSocket& Other);
		~TCPSocket();

		// Bind socket for servers()
		WinsockError Bind();

		// For listening 
		WinsockError Listen(const int& BackLog = SOMAXCONN);	

		// Makes TCPSocket ready for server use
		CallInfo MakeServerReady(const int& BackLog = SOMAXCONN);

		// Connect to server or anything that uses TCP
		SOCKET Connect(const std::string& Ip, const int& Port);

		// Accept a connection
		SOCKET Accept(sockaddr* Addr, int* Size);

		// Accpet a connection, Addr = nullptr, Size = nullptr
		SOCKET Accept();

		// Send data to a socket
		WinsockError Send(const std::string& Str);

		// Recv data from a socket
		CallInfoData<std::string> Recv(const int& SizeOfBuffer = 100);

		bool operator==(TCPSocket Other);

		bool operator!=(TCPSocket Other);
	};

	class UDPSocket : public BaseSocket
	{
	public:

		UDPSocket();
		UDPSocket(const SOCKET& Socket);
		UDPSocket(const std::string& Ip, const int& Port);
		~UDPSocket();

		// Bind socket for servers()
		void Bind();

		// Send data to a socket
		WinsockError SendTo(const SOCKET& Socket, const std::string& Ip, const int& Port, const std::string& Str);

		// Send data to a socket
		WinsockError SendTo(const SOCKET& Socket, sockaddr_in& Addr, const std::string& Str);

		// Send data to a socket6
		WinsockError SendTo6(const SOCKET& Socket, const std::string& Ip, const int& Port, const std::string& Str);

		// Send data to a socket6
		WinsockError SendTo6(const SOCKET& Socket, sockaddr_in6& Addr, const std::string& Str);

		// Recv data from a socket
		CallInfoData<std::string> RecvFrom(const SOCKET& Socket, const int& Size, const std::string& Ip, const int& Port);

		// Recv data from a socket
		CallInfoData<std::string> RecvFrom(const SOCKET& Socket, const int& Size, sockaddr_in& Addr);

		// Recv data from a socket6
		CallInfoData<std::string> RecvFrom6(const SOCKET& Socket, const int& Size, const std::string& Ip, const int& Port);

		// Recv data from a socket6
		CallInfoData<std::string> RecvFrom6(const SOCKET& Socket, const int& Size, sockaddr_in6& Addr);

		bool operator==(UDPSocket Other);

		bool operator!=(UDPSocket Other);
	};

}

#endif // SOCKETS_H