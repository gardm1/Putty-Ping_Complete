#include "process.h"

static LPTSTR pchar_to_lptstr(const char* c) {
	if (!c) return NULL;

	// Specify char string c is in utf-8 encoding
	int len = MultiByteToWideChar(CP_UTF8, 0, c, -1, NULL, 0);
	if (len <= 1) {
		fprintf(stderr, "Error: Lenght of char string returns 0 (%ld).\n", GetLastError());
		return NULL;
	}

	LPWSTR wideString = (LPWSTR)malloc(len * sizeof(WCHAR));
	if (!wideString) {
		fprintf(stderr, "Error: Memory allocation failed when converting to wide string (%ld).\n", GetLastError());
		return NULL;
	}

	// Convert the multibyte string to a wide string
	MultiByteToWideChar(CP_UTF8, 0, c, -1, wideString, len);

	return (LPTSTR)wideString;
}

int EXECUTECOMMAND(const char* sargv) {
	LPTSTR args = NULL;
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	DWORD dwProcessId = 0;
	DWORD dwThreadId = 0;
	DWORD dwRetVal = 0;

	args = pchar_to_lptstr(sargv);
	if (!args) {
		fprintf(stderr, "Error: Command args failed to initialize.\n");
		return -1;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(
		NULL,
		args,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	))
	{
		fprintf(stderr, "Error: CreateProcess Failed (%ld).\n", GetLastError());
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		free(args);
		return -1;
	}

#ifdef DEBUG
	printf("GetProcessID -> %d\n", GetProcessId(pi.hProcess));
	printf("GetThreadID -> %d\n", GetThreadId(pi.hThread));
#endif

	dwRetVal = WaitForSingleObject(pi.hProcess, SINGLEOBJTIME);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	free(args);

	if (dwRetVal != WAIT_OBJECT_0) {
		fprintf(stderr, "Error: WaitForSingleObject failed.\n");
		switch (dwRetVal) {
		case WAIT_TIMEOUT:
			printf("\tRequest timed out\n");
			break;
		default:
			printf("\tExtended error returned (%ld)", GetLastError());
			break;
		}
		return -1;
	}
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
		IcmpCloseHandle(hIcmpFile);
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
		IcmpCloseHandle(hIcmpFile);
		return -1;
	}
	free(ReplyBuffer);
	IcmpCloseHandle(hIcmpFile);
	return 0;
}

