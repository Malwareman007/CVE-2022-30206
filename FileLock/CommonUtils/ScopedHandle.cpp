

#include "stdafx.h"
#include "ScopedHandle.h"

static HANDLE Duplicate(HANDLE h)
{
	HANDLE dup;

	if ((h == INVALID_HANDLE_VALUE) || !DuplicateHandle(GetCurrentProcess(), h, GetCurrentProcess(), &dup, 0, FALSE, DUPLICATE_SAME_ACCESS))
	{
		return nullptr;
	}
	else
	{
		return dup;
	}
}

ScopedHandle::ScopedHandle(HANDLE h, bool duplicate)
{	
	if (duplicate)
	{
		g_h = Duplicate(h);
	}
	else
	{
		g_h = h;
	}
}

ScopedHandle::ScopedHandle(const ScopedHandle& other)
{
	g_h = Duplicate(other.g_h);
}

ScopedHandle& ScopedHandle::operator=(const ScopedHandle& other)
{
	if (this != &other)
	{
		g_h = Duplicate(other.g_h);
	}

	return *this;
}

ScopedHandle::ScopedHandle(ScopedHandle&& other)
{
	g_h = other.g_h;
	other.g_h = nullptr;
}

ScopedHandle& ScopedHandle::operator=(ScopedHandle&& other)
{
	if (this != &other)
	{
		g_h = other.g_h;
		other.g_h = nullptr;
	}

	return *this;
}

void ScopedHandle::Close() 
{
	if (IsValid())
	{
		CloseHandle(g_h);
		g_h = nullptr;
	}
}

void ScopedHandle::Reset(HANDLE h)
{
	Close();
	g_h = h;
}

ScopedHandle::~ScopedHandle()
{
	Close();
}
