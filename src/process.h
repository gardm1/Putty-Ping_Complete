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

#define RELEASE
//#define DEBUG

#define ICMPTIME 5000
#define SINGLEOBJTIME 3000

__declspec(dllexport) int __stdcall EXECUTE_COMMAND(const char* argv);

__declspec(dllexport) int __stdcall PING_INET_ADDR(const char* argv);

#endif // !__PROCESS_H