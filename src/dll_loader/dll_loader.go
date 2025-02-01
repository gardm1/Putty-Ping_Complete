package dll_loader 

import (
	"syscall"
	"fmt"
)

type DLLLoader struct {
	dll					*syscall.DLL
	executeCommandProc  *syscall.Proc
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

func (loader *DLLLoader) ExecuteCommand(argv uintptr) error {
	if loader.executeCommandProc == nil {
		return fmt.Errorf("EXECUTE_COMMAND procedure is not loaded")
	}

	ret, _, err := loader.executeCommandProc.Call(argv)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) PingInetAddr(argv uintptr) error {
	if loader.pingInetAddrProc == nil {
		return fmt.Errorf("PING_INET_ADDR procedure is not loaded")
	}

	ret, _, err := loader.pingInetAddrProc.Call(argv)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) EncryptFile(ifp uintptr, ofp uintptr, kfp uintptr) error {
	if loader.encryptFileProc == nil {
		return fmt.Errorf("ENCRYPT_FILE procedure is not loaded")
	}

	ret, _, err := loader.encryptFileProc.Call(ifp, ofp, kfp)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) DecryptFile(ifp uintptr, ofp uintptr, kfp uintptr) error {
	if loader.decryptFileProc == nil {
		return fmt.Errorf("DECRYPT_FILE procedure is not loaded")
	}

	ret, _, err := loader.decryptFileProc.Call(ifp, ofp, kfp)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) UnloadDLL() error {
	if loader.dll != nil {
		return loader.dll.Release()
	}

	return nil
}
