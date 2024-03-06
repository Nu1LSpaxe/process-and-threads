#if 0

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

// InterlockedIncrement64(
// 	LONGLONG volatile *Addend
// );


#define MAX_DOCUMENT 100
#define TOTAL_STAFF 3

// volatile avoid compiler optimization (we want to read the value from memory instead of cache)
volatile LONGLONG Document_Number = 0;
INT Document_Counter[MAX_DOCUMENT];

UINT __stdcall Staff(PVOID lp)
{
	INT StaffID = *(INT*)lp;
	while (Document_Number < MAX_DOCUMENT)
	{
		INT i = InterlockedIncrement64(&Document_Number) - 1;
		printf("Process document %2d by staff %d\n", i, StaffID);
		Document_Counter[i]++;
	}
	return 0;
}

INT main(INT argc, CHAR* argv[])
{
	HANDLE Staffs[TOTAL_STAFF];
	INT StaffIDs[TOTAL_STAFF];

	// ZeroMemory: Fills a block of memory with zeros
	ZeroMemory(Document_Counter, sizeof(Document_Counter));

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

	printf("Check documents state: \n");
	// Check all documents are processed by only one staff
	for (INT i = 0; i < MAX_DOCUMENT; ++i)
	{
		printf("Document %2d is processed by %d staffs\n", i, Document_Counter[i]);
	}

	system("pause");
	return 0;
}

#endif