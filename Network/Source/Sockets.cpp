#include "NHeaders/Sockets.h"

namespace HLnet
{
	BaseSocket::BaseSocket()
		: Socket(SOCKET()), Info4(sockaddr_in()), Info6(sockaddr_in6()), Type(0)
	{
	}

	BaseSocket::BaseSocket(const SOCKET& Socket)
		: Socket(Socket), Info4(sockaddr_in()), Info6(sockaddr_in6()), Type(0)
	{
	}

	inline void BaseSocket::_ZeroMembers()
	{
		Socket = SOCKET();
		Info4 = sockaddr_in();
		Info6 = sockaddr_in6();
		Type = 0;
	}

	inline void BaseSocket::SetSocket(const std::string& Ip, const int& Port)
	{
		_ZeroMembers();

		// Make socket
		this->Type = (int)HLnet::GetTypeFromString(Ip);
		this->Socket = socket(Type, SOCK_STREAM, 0);
		if (Socket == INVALID_SOCKET)
		{
			Errors.push_back(
				SocketCouldNotBeCreated("Proto : TCP, Version : " + std::to_string(Type)));

			return;
		}

		SetSockAddr(Ip, Port);
	}

	inline void BaseSocket::SetSockAddr(const std::string& Ip, const int& Port_)
	{
		switch (this->Type)
		{
		case (int)IPVersion::_4:
			Info4.sin_family = this->Type;
			Info4.sin_port = htons(Port_), this->Port = Port_;
			inet_pton(this->Type, Ip.c_str(), (void*)&this->Info4.sin_addr);
			break;

		case (int)IPVersion::_6:
			Info6.sin6_family = this->Type;
			Info6.sin6_port = htons(Port_), this->Port = Port_;
			inet_pton(this->Type, Ip.c_str(), (void*)&this->Info6.sin6_addr);
			break;

		default:
			Errors.push_back(UnknownError);
		}

	}

	bool BaseSocket::Bad()
	{
		return Socket == INVALID_SOCKET;
	}

	WinsockError BaseSocket::Close()
	{
		return closesocket(Socket);
	}

	SOCKET& BaseSocket::GetSocket()
	{
		return this->Socket;
	}

	sockaddr_in BaseSocket::GetSockAddr()
	{
		return this->Info4;
	}

	sockaddr_in6 BaseSocket::GetSockAddr6()
	{
		return this->Info6;
	}

	int BaseSocket::GetPort()
	{
		return Port;
	}

	int& BaseSocket::GetIPV()
	{
		return Type;
	}

	bool BaseSocket::operator==(BaseSocket Other)
	{
		return this->Socket == Other.Socket;
	}

	bool BaseSocket::operator!=(BaseSocket Other)
	{
		return this->Socket != Other.Socket;
	}

	TCPSocket::TCPSocket()
	{
		Socket = SOCKET();
		Info4 = sockaddr_in();
		Info6 = sockaddr_in6();
		Type = 0;
	}

	TCPSocket::TCPSocket(const SOCKET& Socket_)
	{
		Socket = Socket_;
		Info4 = sockaddr_in();
		Info6 = sockaddr_in6();
		Type = 0;
	}

	TCPSocket::TCPSocket(const std::string& Ip, const int& Port)
	{
		Socket = SOCKET();
		Info4 = sockaddr_in();
		Info6 = sockaddr_in6();
		Type = 0;
		SetSocket(Ip, Port);
	}

	TCPSocket::TCPSocket(BaseSocket& Other)
	{
		Socket = Other.GetSocket();
		Info4 = Other.GetSockAddr();
		Info6 = Other.GetSockAddr6();
		Type = Other.GetIPV();
	}

	TCPSocket::~TCPSocket()
	{
	}

	WinsockError TCPSocket::Bind()
	{
		switch (Type)
		{
		case (int)IPVersion::_4:
			return bind(Socket, (sockaddr*)&Info4, sizeof(Info4));

		case (int)IPVersion::_6:
			return bind(Socket, (sockaddr*)&Info6, sizeof(Info6));

		default:
			return (int)IPVersion::UNKNOWN;
		}
	}

	WinsockError TCPSocket::Listen(const int& BackLog)
	{
		return listen(Socket, BackLog);
	}

	CallInfo TCPSocket::MakeServerReady(const int& BackLog)
	{
		CallInfo Info_;
		Info_.Log.emplace_back(Bind());
		Info_.Log.emplace_back(Listen(BackLog));
		return Info_;
	}

	SOCKET TCPSocket::Connect(const std::string& Ip, const int& Port)
	{
		switch (GetTypeFromString(Ip))
		{
		case IPVersion::_4:
			return connect(Socket, (sockaddr*)&MakeAddr(Ip, Port), sizeof(sockaddr_in));

		case IPVersion::_6:
			return connect(Socket, (sockaddr*)&MakeAddr6(Ip, Port), sizeof(sockaddr_in6));

		default:
			return INVALID_SOCKET;
		}
	}

	SOCKET TCPSocket::Accept(sockaddr* Addr, int* Size)
	{
		return NetCall(accept(Socket, Addr, Size));
	}

	SOCKET TCPSocket::Accept()
	{
		return accept(Socket, nullptr, nullptr);
	}

	WinsockError TCPSocket::Send(const std::string& Str)
	{
		return send(Socket, Str.c_str(), static_cast<int>(strlen(Str.c_str())), 0);
	}

	CallInfoData<std::string> TCPSocket::Recv(const int& SizeOfBuffer)
	{
		CallInfoData<std::string> Info_;
		std::vector<char> Buffer(SizeOfBuffer);
		Info_.Log.emplace_back(recv(Socket, &Buffer[0], SizeOfBuffer, 0));
		Info_.Data = std::string().append(Buffer.cbegin(), Buffer.cend());
		return Info_;
	}

	void TCPSocket::PrintInfo()
	{
		std::cout << /*Name from c*/ "Ip(v4): " << IPV4ntop(Info4.sin_addr) << 
			", Ip(v6): " << IPV6ntop(Info6.sin6_addr) << 
			", port: " << Port << '\n';
	}

	bool TCPSocket::operator==(TCPSocket Other)
	{
		return Socket == Other.Socket;
	}

	bool TCPSocket::operator!=(TCPSocket Other)
	{
		return Socket != Other.Socket;
	}

	UDPSocket::UDPSocket()
	{
		Socket = SOCKET();
		Info4 = sockaddr_in();
		Info6 = sockaddr_in6();
		Type = 0;
	}

	UDPSocket::UDPSocket(const SOCKET& Socket_)
	{
		Socket = Socket_;
		Info4 = sockaddr_in();
		Info6 = sockaddr_in6();
		Type = 0;
	}

	UDPSocket::UDPSocket(const std::string& Ip, const int& Port)
	{
		Socket = SOCKET();
		Info4 = sockaddr_in();
		Info6 = sockaddr_in6();
		Type = 0;
		SetSocket(Ip, Port);
	}

	UDPSocket::~UDPSocket()
	{
	}

	void UDPSocket::Bind()
	{
		switch (Type)
		{
		case (int)IPVersion::_4:
			bind(Socket, (sockaddr*)&Info4, sizeof(Info4));
			return;

		case (int)IPVersion::_6:
			bind(Socket, (sockaddr*)&Info6, sizeof(Info6));
			return;

		default:
			return;
		}
	}

	int UDPSocket::SendTo(const SOCKET& Socket_, const std::string& Ip, const int& Port, const std::string& Str)
	{
		sockaddr_in Addr = MakeAddr(Ip, Port);
		return sendto(Socket_, Str.c_str(), sizeof(Str), 0, (sockaddr*)&Addr, sizeof(Addr));;
	}

	int UDPSocket::SendTo(const SOCKET& Socket_, sockaddr_in& Addr, const std::string& Str)
	{
		return sendto(Socket_, Str.c_str(), sizeof(Str), 0, (sockaddr*)&Addr, sizeof(Addr));
	}

	int UDPSocket::SendTo6(const SOCKET& Socket_, const std::string& Ip, const int& Port, const std::string& Str)
	{
		sockaddr_in6 Addr = MakeAddr6(Ip, Port);
		return sendto(Socket_, Str.c_str(), sizeof(Str), 0, (sockaddr*)&Addr, sizeof(Addr));;
	}

	int UDPSocket::SendTo6(const SOCKET& Socket_, sockaddr_in6& Addr, const std::string& Str)
	{
		return sendto(Socket_, Str.c_str(), sizeof(Str), 0, (sockaddr*)&Addr, sizeof(Addr));;
	}

	CallInfoData<std::string> UDPSocket::RecvFrom(const SOCKET& Socket, const int& Size, const std::string& Ip, const int& Port)
	{
		CallInfoData<std::string> Info_;
		std::vector<char> Buffer(Size);
		sockaddr_in Addr = MakeAddr(Ip, Port);
		int SizeOfAddr = sizeof(Addr);
		Info_.Log.emplace_back(recvfrom(Socket, &Buffer[0], Size, 0, (sockaddr*)&Addr, &SizeOfAddr));
		Info_.Data = std::string().append(Buffer.cbegin(), Buffer.cend());
		return Info_;
	}

	CallInfoData<std::string> UDPSocket::RecvFrom(const SOCKET& Socket, const int& Size, sockaddr_in& Addr)
	{
		CallInfoData<std::string> Info_;
		std::vector<char> Buffer(Size);
		int SizeOfAddr = sizeof(Addr);
		Info_.Log.emplace_back(recvfrom(Socket, &Buffer[0], Size, 0, (sockaddr*)&Addr, &SizeOfAddr));
		Info_.Data = std::string().append(Buffer.cbegin(), Buffer.cend());
		return Info_;
	}

	CallInfoData<std::string> UDPSocket::RecvFrom6(const SOCKET& Socket, const int& Size, const std::string& Ip, const int& Port)
	{
		CallInfoData<std::string> Info_;
		std::vector<char> Buffer(Size);
		sockaddr_in6 Addr = MakeAddr6(Ip, Port);
		int SizeOfAddr = sizeof(Addr);
		Info_.Log.emplace_back(recvfrom(Socket, &Buffer[0], Size, 0, (sockaddr*)&Addr, &SizeOfAddr));
		Info_.Data = std::string().append(Buffer.cbegin(), Buffer.cend());
		return Info_;
	}

	CallInfoData<std::string> UDPSocket::RecvFrom6(const SOCKET& Socket, const int& Size, sockaddr_in6& Addr)
	{
		CallInfoData<std::string> Info_;
		std::vector<char> Buffer(Size);
		int SizeOfAddr = sizeof(Addr);
		Info_.Log.emplace_back(recvfrom(Socket, &Buffer[0], Size, 0, (sockaddr*)&Addr, &SizeOfAddr));
		Info_.Data = std::string().append(Buffer.cbegin(), Buffer.cend());
		return Info_;
	}

	bool UDPSocket::operator==(UDPSocket Other)
	{
		return Socket == Other.Socket;
	}

	bool UDPSocket::operator!=(UDPSocket Other)
	{
		return Socket != Other.Socket;
	}

	void ClientLeftMessage(TCPSocket& Socket, const CallInfoData<std::string>& Data)
	{
		int Port = 0;
		std::cout << "Client< " <<
			"ip(v4): " << IPV4ntop(Socket.GetSockAddr().sin_addr) <<
			", ip(v6): " << IPV6ntop(Socket.GetSockAddr6().sin6_addr) <<
			", port: " << Socket.GetPort() <<
			" > Left the connection( " << Data.Log[0] << " ) " << '\n';
	}

	void ClientJoinedMessage(SOCKET Socket)
	{
		std::cout << "Socket < #" << Socket << "> joined!\n";
	}

	void ClientSentMessage(SOCKET Socket, std::string& ClientMessage)
	{
		std::cout << "Socket < #" << Socket << "> sent: " << ClientMessage << '\n';
	}
}
