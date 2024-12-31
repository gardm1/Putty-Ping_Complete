package main

import (
	"log"
	"unsafe"
	"puttyping/src/dll_loader"
)

func stringToCString(s string) *byte {
	b := append([]byte(s), 0)
	return &b[0]
}

func getMemAddress(b *byte) uintptr {
	return uintptr(unsafe.Pointer(b))
}

func UnloadDLL_m(loader *dll_loader.DLLLoader) {
	err := loader.UnloadDLL()
	if err != nil {
		log.Fatalf("Error unloading DLL: %v", err)
	}
}

func main() {
	// cl /LD /Gz process.c /link ws2_32.lib iphlpapi.lib /OUT:process.dll
	// Createprocess needs to take ansi strings --> changed to CreateProcessA, passing in LPSTR, not LPTSTR

	loader := &dll_loader.DLLLoader{}
	err := loader.LoadDLL("process.dll")
	if err != nil {
		log.Fatalf("Error executing command: %v", err)
	}

	command := getMemAddress(stringToCString("calc.exe"))
	err = loader.ExecuteCommand(command)
	if err != nil {
		UnloadDLL_m(loader)
		log.Fatalf("Error executing command: %v", err)
	}

	ip := getMemAddress(stringToCString("1.1.1.1"))
	err = loader.PingInetAddr(ip)
	if err != nil {
		UnloadDLL_m(loader)
		log.Fatalf("Error pinging InetAddr: %v", err)
	}

	UnloadDLL_m(loader)
}


