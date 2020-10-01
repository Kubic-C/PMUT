#pragma once
#ifndef TCPSTREAM_H
#define TCPSTREAM_H

#include "Sockets.h"
#include "Sets.h"
#include <WinUser.h>

namespace HLnet
{
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

		// Get TCPSocket
		TCPSocket& GetTCPSocket() const;

	private:
		std::string InitialMessage;
		TCPSocket ListenSocket;
		Set Master;
		timeval SelectTimer;
	};
}

#endif // TCPSTREAM_H