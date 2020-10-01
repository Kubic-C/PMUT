#pragma once
#ifndef ERROR_H
#define ERROR_H

#include "xError.h"

namespace
{
	Error SocketCouldNotBeCreated ("Socket could not created", "");
	Error UnknownError("An error occured but it could not be specified", "");
	Error ListenSocketIsInvalid("The listening socket is invalid; so the server cannot be ran\n this may due to the port or ip being tooken or invalid", "");
}

#endif // ERROR_H