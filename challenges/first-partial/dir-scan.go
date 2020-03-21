package main

import (
	"fmt"
	"os"
	"path/filepath"
	"log"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) {
	
	nDirectories := 0
	nSymbolicLinks := 0
	nRegFiles := 0
	nTempFiles := 0
	nSockets := 0
	nOthers := 0

	err := filepath.Walk(dir, func (path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}

		lstatfile, err2 := os.Lstat(path)
		if err2 != nil {
			log.Fatal(err2)
		}

		mode := lstatfile.Mode()
		if mode.IsDir() {
			nDirectories++
		} else if mode&os.ModeSymlink != 0 {
			nSymbolicLinks++
		} else if mode&os.ModeTemporary != 0 {
			nTempFiles++
		} else if mode.IsRegular() {
			nRegFiles++
		} else if mode&os.ModeSocket != 0 { 
			nSockets++
		} else {
			nOthers++
		}

		return nil
	})
	if err != nil {
		log.Println(err)
	}

	fmt.Println("+-------------------------------+-------------------------------+")
	fmt.Printf("| %-30s| %-30s|\n", "Path", dir)
	fmt.Println("+-------------------------------+-------------------------------+")
	fmt.Printf("| %-30s| %-30d|\n", "Directories", nDirectories)
	fmt.Printf("| %-30s| %-30d|\n", "Symbolic Links", nSymbolicLinks)
	fmt.Printf("| %-30s| %-30d|\n", "Regular Files", nRegFiles)
	fmt.Printf("| %-30s| %-30d|\n", "Temporary Files", nTempFiles)
	fmt.Printf("| %-30s| %-30d|\n", "Sockets", nSockets)
	fmt.Printf("| %-30s| %-30d|\n", "Others", nOthers)
	fmt.Println("+-------------------------------+-------------------------------+")
	
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
