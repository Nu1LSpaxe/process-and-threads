#if 0

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

// Critical Section: 
// InitializeCriticalSection, EnterCriticalSection, LeaveCriticalSection, DeleteCriticalSection
// lpCriticalSection: A pointer to the critical section object.

#define MAX_DOCUMENT 100
#define TOTAL_STAFF 3

volatile LONGLONG Document_Number = 0;
INT Document_Counter[MAX_DOCUMENT];
// Declare critical section object
CRITICAL_SECTION CriticalSection;

UINT __stdcall Staff(PVOID lp)
{
	INT StaffID = *(INT*)lp;
	while (Document_Number < MAX_DOCUMENT)
	{
		// Enter critical section
		EnterCriticalSection(&CriticalSection);

		// With Critical Section, the following code can also increment DocumentNumber safely
		INT i = Document_Number;
		Sleep(rand() % 2);
		Document_Number++;

		// Leave critical section
		LeaveCriticalSection(&CriticalSection);

		printf("Process document %2d by staff %d\n", i, StaffID);
		Document_Counter[i]++;
	}
	return 0;
}

INT main(INT argc, CHAR* argv[])
{
	HANDLE Staffs[TOTAL_STAFF];
	INT StaffIDs[TOTAL_STAFF];
	srand(GetTickCount64());

	ZeroMemory(Document_Counter, sizeof(Document_Counter));

	// Initialize critical section
	InitializeCriticalSection(&CriticalSection);

	for (size_t i{ 0 }; i < TOTAL_STAFF; ++i)
	{
		StaffIDs[i] = i;
		// CREATE_SUSPENDED: The thread is created in a suspended state, and will not run until the ResumeThread function is called.
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

	printf("Check documents state: \n");
	// Check all documents are processed by only one staff
	for (INT i = 0; i < MAX_DOCUMENT; ++i)
	{
		printf("Document %2d is processed by %d staffs\n", i, Document_Counter[i]);
	}

	// Delete critical section
	DeleteCriticalSection(&CriticalSection);

	system("pause");
	return 0;
}

#endif
