#include "NHeaders\Servers.h"

namespace HLnet
{
	TCPServer::TCPServer()
		: ListenSocket(), Master()
	{
		PrepareTimer();
		ListenSocket.PrintErrors();
		//Threads.first.reserve(100);
		//Threads.second.reserve(100);
	}

	TCPServer::TCPServer(const std::string& Ip, const int& Port)
		: ListenSocket(Ip, Port), Master(), InitialMessage("Welcome to a PMUT server!" + (std::string)"\n\r")
	{
		ListenSocket.MakeServerReady(SOMAXCONN);
		PrepareTimer();
		ListenSocket.PrintErrors();
	}

	TCPServer::TCPServer(const std::string& Ip, const int& Port, const std::string& InitialMessage)
		: ListenSocket(Ip, Port), Master(), InitialMessage(InitialMessage.c_str() + (std::string)"\n\r")
	{
		ListenSocket.MakeServerReady(SOMAXCONN);
		PrepareTimer();
		ListenSocket.PrintErrors();
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
		// Copy from master to prevent the master list from being modified(i.e stop removing sockets that still have live connections)
		Set Copy(Master);
		// See which sockets are ready to be read
		int SocketsReady = NetCall(select(NULL, Copy.GetFdSetPtr(), nullptr, nullptr, &SelectTimer)); 
		for (int I = 0; I < SocketsReady; I++)
		{
			TCPSocket Socket = Copy[I];
			if (Socket == ListenSocket) // New connection
			{
				TCPSocket NewClient = NetCall(ListenSocket.Accept(nullptr, nullptr));
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
				if (MessageInfo.Log[0] == 0 || MessageInfo.Log[0] == -1) // Client disconnected
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

		std::cout << "Server is now running in single thread mode, press(I MEAN SPAM) Q to stop the server and return to command-line\n";

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

	void ClientFrame(Set& MasterSet, TCPSocket Socket, std::shared_mutex& Lock, std::shared_ptr<bool> Done)
	{
		timeval Time;
		Time.tv_sec = 1;
		Time.tv_usec = Time.tv_sec * 1000;

		CallInfoData<std::string> Data;
		Data.Log.push_back(0);
		for (;;)
		{
			Set Copy = MasterSet;
			int SocketReady = select(0, Copy.GetFdSetPtr(), nullptr, nullptr, &Time);
			if (SocketReady >= 1)
			{
				Data = Socket.Recv();
				if (Data.Log[0] >= 1)
				{
					ClientSentMessage(Socket.GetSocket(), Data.Data);
					for (TCPSocket Socket_ : MasterSet)
					{
						if (Socket_ != Socket && Socket_ != MasterSet[0] /*<- Listening socket*/)
						{
							Socket_.Send(Data.Data);
						}
					}
				}
				else if(Data.Log[0] <= 0)
				{
					break;
				}
			}

			if (*Done)
				break;
		}

		ClientLeftMessage(Socket, Data);
		Lock.lock_shared();
		MasterSet.Clear(Socket.GetSocket());
		Socket.Close();
		Lock.unlock_shared();
	}

	void TCPServer::MTFrame()
	{
		// Copy from master to prevent the master list from being modified(i.e stop removing sockets that still have live connections)
		Set Copy(Master);
		// See which sockets are ready to be read
		int SocketsReady = select(NULL, Copy.GetFdSetPtr(), nullptr, nullptr, &SelectTimer);
		for (int I = 0; I < SocketsReady; I++)
		{
			TCPSocket Socket = Copy[I];
			if (Socket == ListenSocket) // New connection
			{
				TCPSocket NewClient = NetCall(ListenSocket.Accept(nullptr, nullptr));
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
					Threads.second.emplace_back(std::make_shared<bool>(false));
					size_t Size = Threads.second.size();
					Threads.first.emplace_back(std::thread(
						&ClientFrame,
						std::ref(Master),
						NewClient,
						std::ref(ThreadLock),
						std::ref(Threads.second[Size - 1])));
				}
			}
		}
	}

	void TCPServer::MTRun()
	{
		_StartUpSet();

		std::cout << "Server is now running in multi-thread mode, press(I MEAN SPAM) Q to stop the server and return to command-line\n";

		while (true)
		{
			if (GetKeyState(0x51) & 0x8000)
			{
				// ALT key is down.
				std::cout << "Quiting server...\n";
				break;
			}

			MTFrame();
		}

		_ClearSet();
		for (size_t I = 0; I < Threads.first.size(); I++)
		{
			*Threads.second[I] = true;
			Threads.first[I].join();
		}
		Threads.first.clear();
		Threads.second.clear();
	}

	void TCPServer::SetZero()
	{
		ListenSocket.Close();
	}

	void TCPServer::operator=(const TCPServer& Other)
	{
		SetZero();
		ListenSocket = Other.ListenSocket;
		InitialMessage = Other.InitialMessage;
		Master = Other.Master;
	}

	TCPSocket& TCPServer::GetTCPSocket() const
	{
		return (TCPSocket&)this->ListenSocket;
	}
}