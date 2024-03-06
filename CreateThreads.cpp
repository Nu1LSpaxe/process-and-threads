#if 0

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>
#include <tchar.h>

// uintptr_t _beginthreadex(
// 	void *security,
// 	unsigned stack_size,
// 	unsigned (__stdcall *start_address)(void *),
// 	void *arglist,
// 	unsigned initflag,
// 	unsigned *thrdaddr
// );
// 
// security: A pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle can be inherited by child processes.
// stack_size: The size of the stack, in bytes.
// start_address: A pointer to the application-defined function of type unsigned (__stdcall*)(void*).
// arglist: A pointer to a single argument that is passed to the thread function.
// initflag: The flags that control the creation of the thread.
// thrdaddr: A pointer to a variable that receives the thread identifier.

#define VALID 10

UINT __stdcall Routine(PVOID lp)
{
	INT Number = *(LPINT)lp;
	printf("Thread ID is %d with number: %d\n", GetCurrentThreadId(), Number);
	return 0;
}

INT _tmain(INT argc, TCHAR* argv[])
{
	HANDLE Handles[VALID];
	INT Numbers[VALID];
	INT Number{ 0 };

	for (size_t i{ 0 }; i < VALID; ++i)
	{
		Numbers[i] = i + 1;
		Handles[i] = (HANDLE)_beginthreadex(NULL, 0, Routine, &Numbers[i], 0, NULL);
	}

	// If wait for single thread
	// WaitForSingleObject(Handles[0], INFINITE);
	// If wait for all threads
	WaitForMultipleObjects(VALID, Handles, TRUE, INFINITE);

	// Close handles
	for (auto hd : Handles)
	{
		CloseHandle(hd);
	}

	system("pause");
	return 0;
}

#endif