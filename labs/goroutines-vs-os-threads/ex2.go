package main

// Exercise 9.5

import (
	"fmt"
	"log"
	"os"
	"time"
)

func main() {
	in := make(chan int)
	out := make(chan int)
	total := 0

	file, err := os.Create("goroutine-scheduling-report.txt")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	go connect(in, out)
	go connect(out, in)

	for i := 0; i < 1; i++ {
		in <- 0
		time.Sleep(time.Duration(1) * time.Second)
		x := <-in
		total += x
	}

	_, err = file.WriteString(fmt.Sprintf("Ping pong messages per second: %d", total))
	if err != nil {
		log.Fatal(err)
	}
}

func connect(in chan int, out chan int) {
	for {
		out <- (1 + <-in)
	}
}
