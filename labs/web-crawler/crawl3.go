// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 241.

// Crawl2 crawls web links starting with the command-line arguments.
//
// This version uses a buffered channel as a counting semaphore
// to limit the number of concurrent calls to links.Extract.
//
// Crawl3 adds support for depth limiting.
//
package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"

	"gopl.io/ch5/links"
)

// Type nodes to keep track of depth of crawled links
type nodes struct {
	paths []string
	depth int
}

//!+sema
// tokens is a counting semaphore used to
// enforce a limit of 20 concurrent requests.
var tokens = make(chan struct{}, 20)

func crawl(url string, parentDepth int) nodes {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return nodes{paths: list, depth: parentDepth + 1} // this is so we can return depth too
}

//!-sema

//!+
func main() {
	worklist := make(chan nodes) // Channel now is of type nodes
	var n int                    // number of pending sends to worklist

	if len(os.Args) < 3 {
		log.Println("usage: ./crawl3 -depth=<int> <link ...>")
		os.Exit(1)
	}

	depth, err := strconv.Atoi(os.Args[1][strings.Index(os.Args[1], "=")+1:])
	if err != nil {
		log.Println("error in depth number")
		os.Exit(1)
	}
	// Start with the command-line arguments.
	firstNodes := nodes{paths: os.Args[2:], depth: 0}
	n++
	go func() { worklist <- firstNodes }()

	// Crawl the web concurrently.
	seen := make(map[string]bool)
	for ; n > 0; n-- {
		nodes := <-worklist      // receive the nodes
		if nodes.depth > depth { // Check depth
			continue
		}
		list := nodes.paths
		for _, link := range list {
			if !seen[link] {
				seen[link] = true
				n++
				go func(link string) {
					worklist <- crawl(link, nodes.depth) // pass in the depth
				}(link)
			}
		}

	}
}

//!-
