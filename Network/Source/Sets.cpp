#include "NHeaders/Sets.h"

namespace HLnet
{
	Set::Set()
	{
		Zero();
	}

	void Set::Zero()
	{
		FD_ZERO(&FdSet);
	}

	void Set::Clear(const SOCKET& Socket)
	{
		FD_CLR(Socket, &FdSet);
	}

	void Set::Add(const SOCKET& Socket)
	{
		FD_SET(Socket, &FdSet);
	}

	SOCKET& Set::operator[](const int& Index)
	{
		return FdSet.fd_array[Index];
	}

	fd_set& Set::GetFdSet()
	{
		return this->FdSet;
	}
}