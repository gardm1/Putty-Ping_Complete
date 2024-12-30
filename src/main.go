package main

import (
	"syscall"
	"unsafe"
)

func stringToCString(s string) *byte {
	b := append([]byte(s), 0)
	return &b[0]
}

func getMemAddress(b *byte) uintptr {
	return uintptr(unsafe.Pointer(b))
}

func main() {
	dll := syscall.NewLazyDLL("process.dll")
	proc_EXECUTE_COMMAND := dll.NewProc("EXECUTE_COMMAND")
	proc_PING_INET_ADDR := dll.NewProc("PING_INET_ADDR")

	cCommand := getMemAddress(stringToCString("calc.exe"))

	ret, _, err := proc_EXECUTE_COMMAND.Call(cCommand)

	if ret != 0 {
		println("GO: Call to EXECUTE_COMMAND failed, error:", err)
	} else {
		println("GO: Call to EXECUTE_COMMAND succeeded")
	}

	cIps := getMemAddress(stringToCString("111.222.333.1"))

	ret, _, err = proc_PING_INET_ADDR.Call(cIps)

	if ret != 0 {
		println("GO: Call to PING_INET_ADDR failed, error:", err)
	} else {
		println("GO: Call to PING_INET_ADDR succeeded")
	}

	// cl /LD /Gz process.c /link ws2_32.lib iphlpapi.lib /OUT:process.dll
	// Createprocess needs to take ansi strings --> changed to CreateProcessA, passing in LPSTR, not LPTSTR
}

