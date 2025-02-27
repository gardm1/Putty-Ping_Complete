#include "process.h"

int EXECUTE_COMMAND(const char* argv) {
	//LPTSTR args = NULL;
	LPSTR args = (LPSTR)argv;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	DWORD dwProcessId = 0;
	DWORD dwThreadId = 0;
	BOOL bRetVal = FALSE;
	DWORD dwRetVal = 0;
	DWORD dwError = 0;

	// See if utf-16 is even needed or if you can just use ansi strings. 
	//int buffer_size = MultiByteToWideChar(CP_UTF8, 0, argv, -1, NULL, 0);
	//if (buffer_size == 0) {
	//	fprintf(stderr, "Error: Getting size for wide string failed.\n");
	//	return -1;
	//}
	//WCHAR* buffer = (WCHAR*)malloc(buffer_size * sizeof(WCHAR));
	//MultiByteToWideChar(CP_UTF8, 0, argv, -1, buffer, buffer_size);
	//args = (LPTSTR)buffer;

	//WCHAR buffer[256];
	//MultiByteToWideChar(CP_UTF8, 0, argv, -1, buffer, sizeof(buffer) / sizeof(WCHAR));
	//args = (LPTSTR)buffer;

	// Conclusion: We're using ANSI strings 

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	bRetVal = CreateProcessA(NULL, args, NULL, NULL,
		FALSE, 0, NULL, NULL,
		&si, &pi);

	if (bRetVal != 0) {
#ifdef DEBUG
		printf("DEBUG INFORMATION.\n");
		printf("\tGetProcessID -> %d\n", GetProcessId(pi.hProcess));
		printf("\tGetThreadID -> %d\nEND.\n", GetThreadId(pi.hThread));
#endif
		dwRetVal = WaitForSingleObject(pi.hProcess, SINGLEOBJTIME);
		if (dwRetVal != WAIT_OBJECT_0) {
			fprintf(stderr, "Error: WaitForSingleObject failed.\n");
			switch (dwRetVal) {
			case WAIT_TIMEOUT:
				printf("\tRequest timed out\n");
				break;
			default:
				printf("\tExtended error returned (%ld)\n", GetLastError());
				break;
			}
		}

		if (!CloseHandle(pi.hProcess)) {
			fprintf(stderr, "Error: Failed to close process handle, eer (%ld)\n", GetLastError());
		}

		if (!CloseHandle(pi.hThread)) {
			fprintf(stderr, "Error: Failed to close thread handle, eer (%ld)\n", GetLastError());
		}

	}
	else {
		fprintf(stderr, "Error: Call to CreateProcess failed.\n");
		dwError = GetLastError();
		switch (dwError) {
		case ERROR_FILE_NOT_FOUND:
			printf("\tEmulator not found\n");
			break;
		case ERROR_NOT_ENOUGH_MEMORY:
			printf("\tThere is not enough memory to create the process\n");
			break;
		case ERROR_DLL_NOT_FOUND:
			printf("\tA required DLL was not found\n");
			break;
		default:
			printf("\tExtended error returned (%ld)\n", GetLastError());
			break;
		}

		//free(args);

		return -1;
	}

	//free(args);

	return 0;
}

int PING_INET_ADDR(const char* argv) {
	HANDLE hIcmpFile;
	unsigned long ipaddr = INADDR_NONE;
	DWORD dwRetVal = 0;
	DWORD dwError = 0;
	char SendData[] = "Data Buffer";
	LPVOID ReplyBuffer = NULL;
	DWORD ReplySize = 0;

	ipaddr = inet_addr(argv);
	if (ipaddr == INADDR_NONE) {
		fprintf(stderr, "Error: Invalid Ip address\n");
		return -1;
	}

	hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "\tError: Unable to open handle.\nIcmpCreatefile errorcode (%ld).\n", GetLastError());
		return -1;
	}
	
	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData) + 8;
	ReplyBuffer = malloc(ReplySize);
	if (ReplyBuffer == NULL) {
		fprintf(stderr, "\tError: Unable to allocate memory for reply buffer.\n");
		if (!IcmpCloseHandle(hIcmpFile)) {
			fprintf(stderr, "Error: Failed to close Icmp File handle, eer (%ld)", GetLastError());
		}
		return -1;
	}

	dwRetVal = IcmpSendEcho2(hIcmpFile, NULL, NULL, NULL,
		ipaddr, SendData, sizeof(SendData), NULL,
		ReplyBuffer, ReplySize, ICMPTIME);
	if (dwRetVal != 0) {
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		printf("\tSent icmp message to %s\n", argv);
		if (dwRetVal > 1) {
			printf("\tReceived %ld icmp message responses\n", dwRetVal);
			printf("\tInformation from the first response:\n");
		}
		else {
			printf("\tReceived %ld icmp message response\n", dwRetVal);
			printf("\tInformation from this response:\n");
		}
		printf("\t  Received from %s\n", inet_ntoa(ReplyAddr));
		printf("\t  Status = %ld  ", pEchoReply->Status);
		switch (pEchoReply->Status) {
		case IP_DEST_HOST_UNREACHABLE:
			printf("(Destination host was unreachable)\n");
			break;
		case IP_DEST_NET_UNREACHABLE:
			printf("(Destination Network was unreachable)\n");
			break;
		case IP_REQ_TIMED_OUT:
			printf("(Request timed out)\n");
			break;
		default:
			printf("\n");
			break;
		}

		printf("\t  Roundtrip time = %ld milliseconds\n", pEchoReply->RoundTripTime);
	}
	else {
		fprintf(stderr, "Error: Call to IcmpSendEcho2 failed.\n");
		dwError = GetLastError();
		switch (dwError) {
		case IP_BUF_TOO_SMALL:
			printf("\tReplyBufferSize too small\n");
			break;
		case IP_REQ_TIMED_OUT:
			printf("\tRequest timed out\n");
			break;
		default:
			printf("\tExtended error returned (%ld)\n", dwError);
			break;
		}
		free(ReplyBuffer);
		if (!IcmpCloseHandle(hIcmpFile)) {
			fprintf(stderr, "Error: Failed to close Icmp File handle, eer (%ld)", GetLastError());
		}
		return -1;
	}
	free(ReplyBuffer);
	if (!IcmpCloseHandle(hIcmpFile)) {
		fprintf(stderr, "Error: Failed to close Icmp File handle, eer (%ld)", GetLastError());
	}
	return 0;
}

