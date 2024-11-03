#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#endif

typedef struct Process {
    LPTSTR args;
    ULONG ses_len;
    HANDLE hProcess;
    HANDLE hThread;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD dwProcessId;
    DWORD dwThreadId;
} Process;

void Process_con(Process* process);
void Process_decon(Process* process);

// -------- Process end -------- //

LPTSTR pchar_to_lptstr(const char* c);

int EXECUTECOMMAND(const char* sargv);

int PING_INET_ADDR(const char* argv);

// -------- Global end -------- //

#endif // !__PROCESS_H