#pragma once
#ifndef SETS_H
#define SETS_H

#include "Constants.h"

namespace HLnet
{
	class Set
	{
	public:
		typedef SOCKET* iterator;
		typedef const SOCKET* const_iterator;

		Set();	
		Set(const fd_set& Set);
		Set(Set& Other);

		// FD_ZERO(FdSet)
		void Zero();

		// FD_CLR(Socket, FdSet);
		void Clear(const SOCKET& Socket);

		// Add
		void Add(const SOCKET& Socket);

		SOCKET& operator[](const int& Index);

		iterator begin() { return &FdSet.fd_array[0]; };
		const_iterator begin() const { return &FdSet.fd_array[0]; };
		iterator end() { return &FdSet.fd_array[FdSet.fd_count]; };
		const_iterator end() const { return &FdSet.fd_array[FdSet.fd_count]; };

		fd_set& GetFdSet();

		fd_set* GetFdSetPtr();

	private:
		fd_set FdSet;
	};
}

#endif // SETS_H