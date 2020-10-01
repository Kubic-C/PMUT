#include "NHeaders/Sets.h"

namespace HLnet
{
	Set::Set()
	{
		Zero();
	}

	Set::Set(const fd_set& Set)
		: FdSet()
	{
		Zero();
		this->FdSet = Set;
	}

	Set::Set(Set& Other)
	{
		Zero();
		this->FdSet = Other.GetFdSet();
	}

	void Set::Zero()
	{
		memset(&FdSet.fd_array, 0, sizeof(SOCKET[64]));
		//memset(&FdSet.fd_count, 0, sizeof(unsigned int));
		FD_ZERO(&this->FdSet);
	}

	void Set::Clear(const SOCKET& Socket)
	{
		FD_CLR(Socket, &this->FdSet);
	}

	void Set::Add(const SOCKET& Socket)
	{
		FD_SET(Socket, &this->FdSet);
	}

	SOCKET& Set::operator[](const int& Index)
	{
		return this->FdSet.fd_array[Index];
	}

	fd_set& Set::GetFdSet()
	{
		return this->FdSet;
	}

	fd_set* Set::GetFdSetPtr()
	{
		return &this->FdSet;
	}
}