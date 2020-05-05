// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
)

//!+
func main() {

	if len(os.Args) < 5 {
		log.Println("Usage: ./client -user <user> -server <server:port>")
		os.Exit(1)
	}

	var user string
	var server string

	if os.Args[1] == "-user" && os.Args[3] == "-server" {
		user = os.Args[2]
		server = os.Args[4]
	} else if os.Args[1] == "-server" && os.Args[3] == "-user" {
		server = os.Args[2]
		user = os.Args[4]
	} else {
		log.Println("Check your parameters")
		os.Exit(1)
	}

	log.Println(user)

	conn, err := net.Dial("tcp", server)
	if err != nil {
		log.Fatal(err)
	}
	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	io.WriteString(conn, user)
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
