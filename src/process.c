#include "process.h"

void Process_con(Process* process) {
    process->hProcess = NULL;
    process->hThread = NULL;
    process->dwProcessId = 0;
    process->dwThreadId = 0;

    ZeroMemory(&process->si, sizeof(process->si));
    process->si.cb = sizeof(process->si);
    ZeroMemory(&process->pi, sizeof(process->pi));
    process->args = NULL;
}

void Process_decon(Process* process) {
    CloseHandle(process->pi.hProcess);
    CloseHandle(process->pi.hThread);

    HeapFree(GetProcessHeap(), 0, process->args);
    process->args = NULL;
}

LPTSTR pchar_to_lptstr(const char* c) {
    if (c == NULL) return NULL;

    int len = MultiByteToWideChar(CP_UTF8, 0, c, -1, NULL, 0);
    if (len == 0) {
        printf("Error getting lenght of char string (%ld).\npchar_to_lptstr\n", GetLastError());
        return NULL;
    }

    // Allocate memory --> Could use stack
    LPWSTR wideString = (LPWSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len * sizeof(WCHAR));
    if (wideString == NULL) {
        printf("Memory allocation failed when converting to wide string (%ld).\npchar_to_lptstr\n", GetLastError());
        return NULL;
    }

    // Convert the multibyte string to a wide string
    MultiByteToWideChar(CP_UTF8, 0, c, -1, wideString, len);

    // LPWSTR --> LPTSTR
    return (LPTSTR)wideString;
}


int EXECUTECOMMAND(const char* sargv) {
    Process process;
    Process_con(&process);

    process.args = pchar_to_lptstr(sargv);
    if (process.args == NULL) {
        printf("Command args failed to initialize.\nEXECUTECOMMAND\n");
        Process_decon(&process);
        return 1;
    }

    //wprintf(L"%ls\n", process.args);

    if (!CreateProcess(
        NULL,
        process.args,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &process.si,
        &process.pi
    ))
    {
        printf("CreateProcess Failed (%ld).\nEXECUTECOMMAND\n", GetLastError());
        Process_decon(&process);
        return 1;
    }

#ifdef DEBUG
    printf("GetProcessID -> %d\n", GetProcessId(process.pi.hProcess));
    printf("GetThreadID -> %d\n", GetThreadId(process.pi.hThread));
#endif

    WaitForSingleObject(process.pi.hProcess, INFINITE);
    Process_decon(&process);

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
        printf("Invalid Ip address\n");
        return 1;
    }

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        printf("\tUnable to open handle.\n");
        printf("IcmpCreatefile returned error: %ld\n", GetLastError());
        return 1;
    }

    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData) + 8;
    ReplyBuffer = malloc(ReplySize);
    if (ReplyBuffer == NULL) {
        printf("\tUnable to allocate memory for reply buffer\n");
        return 1;
    }

    dwRetVal = IcmpSendEcho2(hIcmpFile, NULL, NULL, NULL,
        ipaddr, SendData, sizeof(SendData), NULL,
        ReplyBuffer, ReplySize, 5000);
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
        printf("Call to IcmpSendEcho2 failed.\n");
        dwError = GetLastError();
        switch (dwError) {
        case IP_BUF_TOO_SMALL:
            printf("\tReplyBufferSize too small\n");
            break;
        case IP_REQ_TIMED_OUT:
            printf("\tRequest timed out\n");
            break;
        default:
            printf("\tExtended error returned: %ld\n", dwError);
            break;
        }
        free(ReplyBuffer);
        return 1;
    }
    free(ReplyBuffer);
    return 0;
}

