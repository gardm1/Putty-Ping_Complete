package dll_loader 

import (
	"syscall"
	"fmt"
)

type DLLLoader struct {
	dll					*syscall.DLL
	executeCommandProc  *syscall.Proc
	pingInetAddrProc	*syscall.Proc
}

func (loader *DLLLoader) LoadDLL(dllPath string) error {
	var err error
	loader.dll, err = syscall.LoadDLL(dllPath)
	if err != nil {
		return fmt.Errorf("Failed to load DLL %s: %w", dllPath, err)
	}

	loader.executeCommandProc, err = loader.dll.FindProc("EXECUTE_COMMAND")
	if err != nil {
		return fmt.Errorf("Failed to find EXECUTE_COMMAND in %s: %w", dllPath, err)
	}

	loader.pingInetAddrProc, err = loader.dll.FindProc("PING_INET_ADDR")
	if err != nil {
		return fmt.Errorf("Failed to find PING_INET_ADDR in %s: %w", dllPath, err)
	}

	return nil
}

func (loader *DLLLoader) ExecuteCommand(args uintptr) error {
	if loader.executeCommandProc == nil {
		return fmt.Errorf("EXECUTE_COMMAND procedure is not loaded")
	}

	ret, _, err := loader.executeCommandProc.Call(args)

	if ret == 0 {
		return nil
	}

	return err
}

func (loader *DLLLoader) PingInetAddr(args uintptr) error {
	if loader.pingInetAddrProc == nil {
		return fmt.Errorf("PING_INET_ADDR procedure is not loaded")
	}

	ret, _, err := loader.pingInetAddrProc.Call(args)

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
