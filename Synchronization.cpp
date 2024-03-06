#if 0

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>

// CreateEvent(
// 	LPSECURITY_ATTRIBUTES lpEventAttributes,
// 	BOOL bManualReset,
// 	BOOL bInitialState,
// 	LPCWSTR lpName
// );
//
// SetEvent(
// 	HANDLE hEvent
// );

#define MAX_DOCUMENT 100
#define TOTAL 2

volatile LONGLONG Document_Number = 0;
INT Document_Counter[MAX_DOCUMENT];
// Declare event handlers
HANDLE Process1_Done, Process2_Ready, Process2_GetDoc;

UINT __stdcall Process1(PVOID lp)
{
	while (Document_Number < MAX_DOCUMENT)
	{
		printf("Start document %2d by process 1\n", (INT)Document_Number);
		printf("Finish process document %2d by process 1\n", (INT)Document_Number);

		Document_Counter[Document_Number]++;
		// Set event to notify process 2 that process 1 is done
		SetEvent(Process1_Done);
		// Wait for process 2 notify that it is ready to get document
		WaitForSingleObject(Process2_Ready, INFINITE);
		// Wait for process 2 notify that it has got the document
		WaitForSingleObject(Process2_GetDoc, INFINITE);
		Document_Number++;
	}
	return 0;
}

UINT __stdcall Process2(PVOID lp)
{
	SetEvent(Process2_Ready);
	while (Document_Number < MAX_DOCUMENT)
	{
		// Wait for process 1 to finish
		WaitForSingleObject(Process1_Done, INFINITE);

		printf("Start document %2d by process 2\n", (INT)Document_Number);

		// Notify process 1 that process 2 has got the document
		SetEvent(Process2_GetDoc);

		printf("Finish process document %2d by process 2\n",(INT)Document_Number);
		Document_Counter[Document_Number]++;

		// Set event to notify process 1 that process 2 has got the document
		SetEvent(Process2_Ready);
	}
	return 0;
}

INT main(INT argc, CHAR* argv[])
{
	HANDLE Handles[TOTAL];
	ZeroMemory(Document_Counter, sizeof(Document_Counter));

	// Create event handlers
	Process1_Done = CreateEvent(NULL, FALSE, FALSE, NULL);
	Process2_Ready = CreateEvent(NULL, FALSE, FALSE, NULL);
	Process2_GetDoc = CreateEvent(NULL, FALSE, FALSE, NULL);

	Handles[0] = (HANDLE)_beginthreadex(NULL, 0, Process1, NULL, 0, NULL);
	Handles[1] = (HANDLE)_beginthreadex(NULL, 0, Process2, NULL, 0, NULL);

	if (Process1_Done)
	{
		WaitForSingleObject(Process1_Done, INFINITE);
		CloseHandle(Process1_Done);
	}
	if (Process2_Ready)
	{
		WaitForSingleObject(Process2_Ready, INFINITE);
		CloseHandle(Process2_Ready);
	}
	if (Process2_GetDoc)
	{
		WaitForSingleObject(Process2_GetDoc, INFINITE);
		CloseHandle(Process2_GetDoc);
	}

	for (INT i = 0; i < MAX_DOCUMENT; ++i)
	{
		printf("Document %2d is processed by %d staffs\n", i, Document_Counter[i]);
	}

	system("pause");
	return 0;
}

#endif