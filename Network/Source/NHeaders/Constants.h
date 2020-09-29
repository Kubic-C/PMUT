#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib")

#include <vector>
#include <iostream>
#include <string>

#ifdef DEBUG
#define NetCall(x) x; int Error_ = WSAGetLastError(); \
if(Error_ != 0) \
	std::cout << "Line : " <<  __LINE__ << ", Dir : " << __FILE__ << ", WinSockError : \n" << Error_ << std::endl;
#else
#define NetCall(x) x;
#endif // DEBUG

#endif // CONSTANTS_H 