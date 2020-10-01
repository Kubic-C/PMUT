#include "NHeaders\Servers.h"

namespace HLnet
{
	TCPServer::TCPServer()
		: ListenSocket(), Master()
	{
		PrepareTimer();
	}

	TCPServer::TCPServer(const std::string& Ip, const int& Port)
		: ListenSocket(Ip, Port), Master(), InitialMessage("Welcome to a PMUT server!")
	{
		ListenSocket.MakeServerReady(SOMAXCONN);
		PrepareTimer();
	}

	TCPServer::TCPServer(const std::string& Ip, const int& Port, const std::string& InitialMessage)
		: ListenSocket(Ip, Port), Master(), InitialMessage(InitialMessage + '\n')
	{
		ListenSocket.MakeServerReady(SOMAXCONN);
		PrepareTimer();
	}

	void TCPServer::_ClearSet()
	{
		for (TCPSocket Socket : Master) // Close all connections
		{
			if (Socket != ListenSocket.GetSocket())
				Socket.Close();
		}
		Master.Zero();
	}

	void TCPServer::_StartUpSet()
	{
		Master.Zero();
		Master.Add(ListenSocket.GetSocket());
	}

	void TCPServer::PrepareTimer()
	{
		SelectTimer.tv_sec = 1;
		SelectTimer.tv_usec = SelectTimer.tv_sec * 1000;
	}

	inline void TCPServer::STFrame()
	{
		enum Index
		{
			BytesRecv = 0 
		};
		
		// Copy from master to prevent the master list from being modified(i.e stop removing sockets that still have live connections)
		Set Copy(Master);
		// See which sockets are ready to be read
		int SocketsReady = NetCall(select(NULL, Copy.GetFdSetPtr(), nullptr, nullptr, &SelectTimer)); 
		for (int I = 0; I < SocketsReady; I++)
		{
			TCPSocket Socket = Copy[I];
			if (Socket == ListenSocket) // New connection
			{
				sockaddr AddrInfo;
				int Size = sizeof(AddrInfo);
				TCPSocket NewClient = NetCall(ListenSocket.Accept(&AddrInfo, &Size));
				if (NewClient.Bad()) // Client is bad; so remove it
				{
					std::cout << "Incoming connection was invalid, so it was closed\n";
					NewClient.~TCPSocket();
					continue;
				}
				else // Client is good; so add it!
				{
					NewClient.Send(InitialMessage);
					Master.Add(NewClient.GetSocket());
					ClientJoinedMessage(NewClient.GetSocket());
				}
			}
			else // Message...
			{
				CallInfoData<std::string> MessageInfo = NetCall(Socket.Recv());
				if (MessageInfo.Log[Index::BytesRecv] == 0 || MessageInfo.Log[Index::BytesRecv] == -1) // Client disconnected
				{
					ClientLeftMessage(Socket, MessageInfo);
					Master.Clear(Socket.GetSocket());
					Socket.Close();
					continue;
				}
				else // Message is ok; so send the message to all other connected clients!
				{
					ClientSentMessage(Socket.GetSocket(), MessageInfo.Data);

					for (TCPSocket Socket_ : Master)
					{
						if (Socket_ != Socket && Socket_ != ListenSocket)
						{
							NetCall(Socket_.Send(MessageInfo.Data));
						}
					}
				}
			}
		}
	} // Func end

	void TCPServer::STRun()
	{
		_StartUpSet();

		std::cout << "Server is now running in single thread mode, press ALT+Q to stop the server and return command-line\n";

		while (true)
		{
			if (GetKeyState(0x51) & 0x8000)
			{
				// ALT key is down.
				std::cout << "Quiting server...\n";
				break;
			}

			STFrame();
		}

		_ClearSet();
	}

	TCPSocket& TCPServer::GetTCPSocket() const 
	{
		return (TCPSocket&)this->ListenSocket;
	}
}