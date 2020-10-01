#pragma once
#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include "Sockets.h"
#include "Sets.h"
#include <WinUser.h>
#include <thread>
#include <shared_mutex>

namespace HLnet
{
	static void ClientFrame(Set& MasterSet, TCPSocket Socket, std::shared_mutex& Lock, std::shared_ptr<bool> Done);

	class TCPServer
	{
	public:
		TCPServer();
		TCPServer(const std::string& Ip, const int& Port);
		TCPServer(const std::string& Ip, const int& Port, const std::string& InitialMessage);

		// Close & clear all
		void _ClearSet();

		// Start set
		void _StartUpSet();

		void PrepareTimer();

		// Single Thread Frame
		inline void STFrame();

		// Single Thread Run
		void STRun();

		// Multi-thread frame
		void MTFrame();

		// Main run of multi-thread
		void MTRun();

		void SetZero();

		void operator=(const TCPServer& Other);

		// Get TCPSocket
		TCPSocket& GetTCPSocket() const;

	private:
		std::string InitialMessage;
		TCPSocket ListenSocket;
		Set Master;
		timeval SelectTimer;
		std::shared_mutex ThreadLock;
		std::pair <std::vector <std::thread>, std::vector<std::shared_ptr<bool>>> Threads;
	};
}

#endif // TCPSTREAM_H