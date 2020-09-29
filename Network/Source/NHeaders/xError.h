#pragma once
#ifndef XERROR_H
#define XERROR_H

#include "Constants.h"

struct Error
{
	const char* zError;
	std::string Cause;

	Error(const char* Error_, std::string Cause_)
		: zError(Error_), Cause(Cause_)
	{
	}

	Error& operator()(const std::string& Cause_)
	{
		Cause = Cause_;
		return (*this);
	}

	template<typename T>
	Error& operator[](T Cause_)
	{
		Cause = std::to_string(Cause_);
		return (*this);
	}
};

#endif // XERROR_H