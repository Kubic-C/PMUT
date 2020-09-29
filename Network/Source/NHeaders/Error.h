#pragma once
#ifndef ERROR_H
#define ERROR_H

#include "xError.h"

namespace
{
	Error SocketCouldNotBeCreated ("Socket could not created", "");
	Error UnknownError("An error occured but it could not be specified", "");
}

#endif // ERROR_H