

#include "stdafx.h"
#include "CommonUtils.h"
#include "ntimports.h"

HANDLE CreateObjectDirectory(HANDLE hRoot, LPCWSTR dirname, HANDLE hShadow)
{
	DEFINE_NTDLL(RtlInitUnicodeString);
	DEFINE_NTDLL(NtCreateDirectoryObjectEx);

	OBJECT_ATTRIBUTES obj_attr;
	UNICODE_STRING obj_name;

	if (dirname)
	{
		fRtlInitUnicodeString(&obj_name, dirname);
		InitializeObjectAttributes(&obj_attr, &obj_name, OBJ_CASE_INSENSITIVE, hRoot, nullptr);
	}
	else
	{
		InitializeObjectAttributes(&obj_attr, nullptr, OBJ_CASE_INSENSITIVE, hRoot, nullptr);
	}

	HANDLE h = nullptr;
	NTSTATUS status = fNtCreateDirectoryObjectEx(&h, DIRECTORY_ALL_ACCESS, &obj_attr, hShadow, 0);
	if (status == 0)
	{
		return h;
	}
	else
	{
		SetLastError(NtStatusToDosError(status));
		return nullptr;
	}
}

HANDLE OpenObjectDirectory(HANDLE hRoot, LPCWSTR dirname)
{
	DEFINE_NTDLL(RtlInitUnicodeString);
	DEFINE_NTDLL(NtOpenDirectoryObject);

	OBJECT_ATTRIBUTES obj_attr;
	UNICODE_STRING obj_name;

	fRtlInitUnicodeString(&obj_name, dirname);

	InitializeObjectAttributes(&obj_attr, &obj_name, OBJ_CASE_INSENSITIVE, hRoot, nullptr);

	HANDLE h = nullptr;

	NTSTATUS status = fNtOpenDirectoryObject(&h, MAXIMUM_ALLOWED, &obj_attr);
	if (status == 0)
	{
		return h;
	}
	else
	{
		SetLastError(NtStatusToDosError(status));
		return nullptr;
	}
}