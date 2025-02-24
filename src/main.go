package main

import (
    "fmt"
    "log"
    "os"
    "path/filepath"

    "github.com/spf13/cobra"
    "puttyping/src/dll_loader"
)

var loader = &dll_loader.DLLLoader{}

func init() {
    exePath, err := os.Executable()
    if err != nil {
        log.Fatalf("Failed to get executable path: %v", err)
    }
    exeDir := filepath.Dir(exePath)
    dllPath := filepath.Join(exeDir, "\\", "toolbox.dll")

    if err := loader.LoadDLL(dllPath); err != nil {
        log.Fatalf("Failed to load DLL from %s: %v", dllPath, err)
    }
}

func main() {
    defer loader.UnloadDLL()

    var rootCmd = &cobra.Command{
        Use:   "pping",
        Short: "Tool to interact with putty sessions",
        Long: `
A tool to interact with putty sessions including
ExecuteCommand, PingInetAddr, EncryptFile, and DecryptFile.`,
    }

    var execCmd = &cobra.Command{
        Use:   "exec [command]",
        Short: "Runs ExecuteCommand with a string argument",
        Args:  cobra.MinimumNArgs(1),
        Run: func(cmd *cobra.Command, args []string) {
            if err := loader.ExecuteCommand(args[0]); err != nil {
                log.Fatalf("Failed to execute command: %v", err)
            }
            fmt.Printf("Successfully executed command: %s\n", args[0])
        },
    }

    var pingCmd = &cobra.Command{
        Use:   "ping [address]",
        Short: "Runs PingInetAddr with an address",
        Args:  cobra.MinimumNArgs(1),
        Run: func(cmd *cobra.Command, args []string) {
            if err := loader.PingInetAddr(args[0]); err != nil {
                log.Fatalf("Failed to ping address: %v", err)
            }
            fmt.Printf("Successfully pinged: %s\n", args[0])
        },
    }

    var encryptCmd = &cobra.Command{
        Use:   "encrypt [input_file] [output_file] [key_file]",
        Short: "Encrypts a file",
        Args:  cobra.MinimumNArgs(3),
        Run: func(cmd *cobra.Command, args []string) {
            if err := loader.EncryptFile(args[0], args[1], args[2]); err != nil {
                log.Fatalf("Failed to encrypt file: %v", err)
            }
            fmt.Printf("Successfully encrypted %s to %s with key %s\n", args[0], args[1], args[2])
        },
    }

    var decryptCmd = &cobra.Command{
        Use:   "decrypt [input_file] [output_file] [key_file]",
        Short: "Decrypts a file",
        Args:  cobra.MinimumNArgs(3),
        Run: func(cmd *cobra.Command, args []string) {
            if err := loader.DecryptFile(args[0], args[1], args[2]); err != nil {
                log.Fatalf("Failed to decrypt file: %v", err)
            }
            fmt.Printf("Successfully decrypted %s to %s with key %s\n", args[0], args[1], args[2])
        },
    }

    rootCmd.AddCommand(execCmd, pingCmd, encryptCmd, decryptCmd)

    if err := rootCmd.Execute(); err != nil {
        log.Fatal(err)
    }
}

