

#include "stdafx.h"
#include "CommonUtils.h"
#include "ntimports.h"

HANDLE CreateSymlink(HANDLE root, LPCWSTR linkname, LPCWSTR targetname)
{
	DEFINE_NTDLL(RtlInitUnicodeString);
	DEFINE_NTDLL(NtCreateSymbolicLinkObject);
	
	OBJECT_ATTRIBUTES objAttr;
	UNICODE_STRING name;
	UNICODE_STRING target;

	fRtlInitUnicodeString(&name, linkname);
	fRtlInitUnicodeString(&target, targetname);

	InitializeObjectAttributes(&objAttr, &name, OBJ_CASE_INSENSITIVE, root, nullptr);	

	HANDLE hLink;

	NTSTATUS status = fNtCreateSymbolicLinkObject(&hLink, 
		SYMBOLIC_LINK_ALL_ACCESS, &objAttr, &target);
	if (status == 0)
	{
		DebugPrintf("Opened Link %ls -> %ls: %p\n", linkname, targetname, hLink);
		return hLink;
	}
	else
	{
		SetLastError(NtStatusToDosError(status));
		return nullptr;
	}
}

HANDLE OpenSymlink(HANDLE root, LPCWSTR linkname)
{
	DEFINE_NTDLL(RtlInitUnicodeString);
	DEFINE_NTDLL(NtOpenSymbolicLinkObject);

	OBJECT_ATTRIBUTES objAttr;
	UNICODE_STRING name;	

	fRtlInitUnicodeString(&name, linkname);	

	InitializeObjectAttributes(&objAttr, &name, OBJ_CASE_INSENSITIVE, root, nullptr);

	HANDLE hLink;

	NTSTATUS status = fNtOpenSymbolicLinkObject(&hLink,
		SYMBOLIC_LINK_ALL_ACCESS, &objAttr);
	if (status == 0)
	{		
		return hLink;
	}
	else
	{
		SetLastError(NtStatusToDosError(status));
		return nullptr;
	}
}