#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <tchar.h>

// CreateProcess(
// 	LPCTSTR lpApplicationName,
// 	LPTSTR lpCommandLine,
// 	LPSECURITY_ATTRIBUTES lpProcessAttributes,	
// 	LPSECURITY_ATTRIBUTES lpThreadAttributes,
// 	BOOL bInheritHandles,		
// 	DWORD dwCreationFlags,
// 	LPVOID lpEnvironment,
// 	LPCTSTR lpCurrentDirectory,
// 	LPSTARTUPINFO lpStartupInfo,	// Message to be sent to the new process
// 	LPPROCESS_INFORMATION lpProcessInformation	// New process information
// );

INT _tmain(INT argc, TCHAR* argv[])
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	// Create a new process
	if (!CreateProcess(
		_T("C:\\Windows\\System32\\calc.exe"),	// Application name
		NULL,	// Command line
		NULL,	// Process attributes
		NULL,	// Thread attributes
		FALSE,	// Inherit handles
		0,		// Creation flags
		NULL,	// Environment
		NULL,	// Current directory
		&si,	// Startup info
		&pi		// Process information
	))
	{
		_tprintf(_T("Create process failed (%d)\n"), GetLastError());
		return 1;
	}

	// Close process and thread handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}