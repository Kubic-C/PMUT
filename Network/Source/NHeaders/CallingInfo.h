#pragma once
#ifndef CALLING_INFO_H
#define CALLING_INFO_H

#include "Constants.h"

namespace HLnet
{
	typedef int WinsockError;
	
	template<typename T>
	struct CallInfoData
	{
		T Data;
		std::vector<int> Log;
	};

	struct CallInfo
	{
		std::vector<int> Log;
	};
}

#endif // CALLING_INFO_H