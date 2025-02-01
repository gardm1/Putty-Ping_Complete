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
	loader := &dll_loader.DLLLoader{}
	err := loader.LoadDLL("toolbox.dll")
	if err != nil {
		log.Fatalf("Loading DLL failed: %v", err)
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

	input_file_path := getMemAddress(stringToCString("bin\\test.txt"))
	output_file_path := getMemAddress(stringToCString("bin\\en_test.txt"))
	decrypted_file_path := getMemAddress(stringToCString("bin\\de_test.txt"))
	key_file_path := getMemAddress(stringToCString("bin\\_key_"))

	err = loader.EncryptFile(input_file_path, output_file_path, key_file_path)
	if err != nil {
		UnloadDLL_m(loader)
		log.Fatalf("Error encrypting file: %v", err)
	}
	

	err = loader.DecryptFile(output_file_path, decrypted_file_path, key_file_path)
	if err != nil {
		UnloadDLL_m(loader)
		log.Fatalf("Error decrypting file: %v", err)
	}
	

	UnloadDLL_m(loader)
}


