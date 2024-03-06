#if 0

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>


// Semaphore maintains a count between zero and a specified maximum value.
// 
// CreateSemaphore(
// 	LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
// 	LONG lInitialCount,
// 	LONG lMaximumCount,
// 	LPCWSTR lpName
// );

#define MAX_CUSTOMER 100
#define TOTAL_CLERK 4

volatile LONGLONG Customer_Number = 0;
// How much customers(clerks) are served(working)
volatile LONGLONG Clerk_Number = 0;
// Declare semaphore object
HANDLE Clerk_Semaphore = NULL;
INT Customer_Counter[MAX_CUSTOMER];

UINT __stdcall Clerk(PVOID lp)
{
	INT ClerkID = *(INT*)lp;
	while (Customer_Number < MAX_CUSTOMER)
	{
		// Wait for semaphore
		WaitForSingleObject(Clerk_Semaphore, INFINITE);

		INT i = InterlockedIncrement64(&Customer_Number) - 1;
		printf("Serve Customer %2d by Clerk %d -------- (available clerks: %d)\n", i, ClerkID, (INT)InterlockedIncrement64(&Clerk_Number));
		Sleep(rand() % 4);
		printf("Serve Customer %2d by Clerk %d done --- (still serving: %d)\n", i, ClerkID, (INT)InterlockedDecrement64(&Clerk_Number));

		// Release semaphore
		ReleaseSemaphore(Clerk_Semaphore, 1, NULL);
		Customer_Counter[i]++;
	}
	return 0;
}

INT main(INT argc, CHAR* argv[])
{
	HANDLE Clerks[TOTAL_CLERK];
	INT ClerkIDs[TOTAL_CLERK];
	srand(GetTickCount64());

	// Create semaphore object
	Clerk_Semaphore = CreateSemaphore(NULL, TOTAL_CLERK, TOTAL_CLERK, NULL);
	ZeroMemory(Customer_Counter, sizeof(Customer_Counter));

	for (size_t i{ 0 }; i < TOTAL_CLERK; ++i)
	{
		ClerkIDs[i] = i;
		Clerks[i] = (HANDLE)_beginthreadex(NULL, 0, Clerk, &ClerkIDs[i], CREATE_SUSPENDED, NULL);
	}

	printf("Clerks are all ready: \n");
	for (auto clerk : Clerks)
	{
		ResumeThread(clerk);
	}

	WaitForMultipleObjects(TOTAL_CLERK, Clerks, TRUE, INFINITE);
	for (INT i = 0; i < MAX_CUSTOMER; ++i)
	{
		printf("Customer %2d is processed by %d clerks\n", i, Customer_Counter[i]);
	}

	CloseHandle(Clerk_Semaphore);

	system("pause");
	return 0;
}

#endif