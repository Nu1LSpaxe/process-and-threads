#if 0

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

// Mutex vs Critical Section
// Mutex: Can be used across processes, deal with abandoned state
// Critical Section: Can be used only within a process
// 
// CreateMutex(
// 	LPSECURITY_ATTRIBUTES lpMutexAttributes,
// 	BOOL bInitialOwner,
// 	LPCWSTR lpName
// );
// 
// ReleaseMutex(
// 	HANDLE hMutex
// );

#define TIMES 100
#define TOTAL_STAFF 5

// Declare mutex object
HANDLE hMutex;

UINT __stdcall Staff(PVOID lp)
{
	INT StaffID = *(INT*)lp;
	for (INT i = 0; i < TIMES; ++i)
	{
		// Wait for mutex
		WaitForSingleObject(hMutex, INFINITE);

		printf("Staff %d entered.\n", StaffID);
		Sleep(rand() % 2);
		printf("Staff %d left.\n", StaffID);

		// Release mutex
		ReleaseMutex(hMutex);
	}
	return 0;
}

INT main(INT argc, CHAR* argv[])
{
	HANDLE Staffs[TOTAL_STAFF];
	INT StaffIDs[TOTAL_STAFF];
	srand(GetTickCount64());

	// Create mutex object
	hMutex = CreateMutex(NULL, FALSE, NULL);


	for (size_t i{ 0 }; i < TOTAL_STAFF; ++i)
	{
		StaffIDs[i] = i;
		Staffs[i] = (HANDLE)_beginthreadex(NULL, 0, Staff, &StaffIDs[i], CREATE_SUSPENDED, NULL);
	}

	printf("Staffs are all ready: \n");
	for (auto staff : Staffs)
	{
		ResumeThread(staff);
	}

	WaitForMultipleObjects(TOTAL_STAFF, Staffs, TRUE, INFINITE);
	for (auto staff : Staffs)
	{
		CloseHandle(staff);
	}

	// Close mutex object
	CloseHandle(hMutex);

	system("pause");
	return 0;
}

#endif