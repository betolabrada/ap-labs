// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"fmt"
	"io"
	"log"
	"net"
	"os"
	"time"
)

func getName() string {

	return "City"
}

func handleConn(c net.Conn, loc *time.Location) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, fmt.Sprintf("%-16s : %s", loc.String(), time.Now().In(loc).Format("15:04:05\n")))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("usage:(TZ=US/Eastern) go run clock2 -port <port> &")
		os.Exit(1)
	}

	port := os.Args[2]
	tz := os.Getenv("TZ")

	loc, err := time.LoadLocation(tz)
	if err != nil {
		log.Fatal(err)
	}

	listener, err := net.Listen("tcp", "localhost:"+port)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, loc) // handle connections concurrently
	}
}
