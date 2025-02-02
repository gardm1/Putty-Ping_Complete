package dll_loader 

import (
	"syscall"
	"unsafe"
	"fmt"
	"log"
)

func stringToCString(s string) *byte {
	b := append([]byte(s), 0)
	return &b[0]
}

func getMemAddress(b *byte) uintptr {
	return uintptr(unsafe.Pointer(b))
}

type DLLLoader struct {
	dll					*syscall.DLL
	executeCommandProc	*syscall.Proc
	pingInetAddrProc	*syscall.Proc
	encryptFileProc		*syscall.Proc
	decryptFileProc		*syscall.Proc
}

func (loader *DLLLoader) LoadDLL(dllPath string) error {
	var err error
	loader.dll, err = syscall.LoadDLL(dllPath)
	if err != nil {
		return fmt.Errorf("Failed to load DLL %s: %w", dllPath, err)
	}

	loader.executeCommandProc, err = loader.dll.FindProc("_dll_EXECUTE_COMMAND")
	if err != nil {
		return fmt.Errorf("Failed to find EXECUTE_COMMAND in %s: %w", dllPath, err)
	}

	loader.pingInetAddrProc, err = loader.dll.FindProc("_dll_PING_INET_ADDR")
	if err != nil {
		return fmt.Errorf("Failed to find PING_INET_ADDR in %s: %w", dllPath, err)
	}

	loader.encryptFileProc, err = loader.dll.FindProc("_dll_ENCRYPT_FILE")
	if err != nil {
		return fmt.Errorf("Failed to find ENCRYPT_FILE in %s: %w", dllPath, err)
	}

	loader.decryptFileProc, err = loader.dll.FindProc("_dll_DECRYPT_FILE")
	if err != nil {
		return fmt.Errorf("Failed to find DECRYPT_FILE in %s: %w", dllPath, err)
	}

	return nil
}

func (loader *DLLLoader) ExecuteCommand(argv string) error {
	if loader.executeCommandProc == nil {
		return fmt.Errorf("EXECUTE_COMMAND procedure is not loaded")
	}

	var c_argv uintptr = getMemAddress(stringToCString(argv))

	ret, _, err := loader.executeCommandProc.Call(c_argv)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) PingInetAddr(argv string) error {
	if loader.pingInetAddrProc == nil {
		return fmt.Errorf("PING_INET_ADDR procedure is not loaded")
	}

	var c_argv uintptr = getMemAddress(stringToCString(argv))

	ret, _, err := loader.pingInetAddrProc.Call(c_argv)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) EncryptFile(input_file_path string, output_file_path string, key_file_path string) error {
	if loader.encryptFileProc == nil {
		return fmt.Errorf("ENCRYPT_FILE procedure is not loaded")
	}

	var c_input_file_path uintptr = getMemAddress(stringToCString(input_file_path))
	var c_output_file_path uintptr = getMemAddress(stringToCString(output_file_path))
	var c_key_file_path uintptr = getMemAddress(stringToCString(key_file_path))

	ret, _, err := loader.encryptFileProc.Call(c_input_file_path, c_output_file_path, c_key_file_path)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) DecryptFile(input_file_path string, output_file_path string, key_file_path string) error {
	if loader.decryptFileProc == nil {
		return fmt.Errorf("DECRYPT_FILE procedure is not loaded")
	}

	var c_input_file_path uintptr = getMemAddress(stringToCString(input_file_path))
	var c_output_file_path uintptr = getMemAddress(stringToCString(output_file_path))
	var c_key_file_path uintptr = getMemAddress(stringToCString(key_file_path))

	ret, _, err := loader.decryptFileProc.Call(c_input_file_path, c_output_file_path, c_key_file_path)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) UnloadDLL() {
	var err error
	if loader.dll != nil {
		err = loader.dll.Release()
	}

	if err != nil {
		log.Fatalf("Error unloading DLL: %v", err)
	}
}
