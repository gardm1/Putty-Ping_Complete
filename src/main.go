package main

import (
	"log"
	"puttyping/src/dll_loader"
)

func main() {
	loader := &dll_loader.DLLLoader{}
	err := loader.LoadDLL("toolbox.dll")
	if err != nil {
		log.Fatalf("Loading DLL failed: %v", err)
	}

	err = loader.ExecuteCommand("calc.exe")
	if err != nil {
		loader.UnloadDLL()
		log.Fatalf("Error executing command: %v", err)
	}

	err = loader.PingInetAddr("1.1.1.1")
	if err != nil {
		loader.UnloadDLL()
		log.Fatalf("Error pinging InetAddr: %v", err)
	}

	input_file_path := "bin\\test.txt"
	output_file_path := "bin\\en_test.txt"
	decrypted_file_path := "bin\\de_test.txt"
	key_file_path := "bin\\_key_"

	err = loader.EncryptFile(input_file_path, output_file_path, key_file_path)
	if err != nil {
		loader.UnloadDLL()
		log.Fatalf("Error encrypting file: %v", err)
	}
	
	err = loader.DecryptFile(output_file_path, decrypted_file_path, key_file_path)
	if err != nil {
		loader.UnloadDLL()
		log.Fatalf("Error decrypting file: %v", err)
	}

	loader.UnloadDLL()
}


