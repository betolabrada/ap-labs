// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"math/rand"
	"net"
	"os"
	"strconv"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct {
	name      string
	ip        string
	admin     bool
	ch        chan string
	notKicked bool
}

var (
	entering  = make(chan client)
	leaving   = make(chan client)
	messages  = make(chan string)     // all incoming client messages
	users     = make([]*user, 0, 100) // current users
	serverwho = "irc-server > "
)

// Initialize struct
func newUser(username, ip string) *user {
	return &user{name: username, ip: ip}
}

// Deletes user from list if found
func deleteUser(cli client) {
	for i, user := range users {
		if user.ch == cli {
			users = append(users[:i], users[i+1:]...)
		}
	}
}

// Get all user names available into one single string
func getAllUsers() string {
	switch len(users) {
	case 0:
		return ""
	case 1:
		return users[0].name
	}
	sep := ", "
	n := len(sep) * (len(users) - 1)
	for i := 0; i < len(users); i++ {
		n += len(users[i].name)
	}

	var b strings.Builder
	b.Grow(n)
	b.WriteString(users[0].name)
	for _, s := range users[1:] {
		b.WriteString(sep)
		b.WriteString(s.name)
	}
	return serverwho + b.String()
}

// Get user from available users by fidning its name
func getUserByName(name string) *user {
	for _, user := range users {
		if user.name == name {
			return user
		}
	}
	return nil
}

// Message specific user
func messageUser(dst *user, who string, message string) string {
	if dst != nil {
		dst.ch <- who + "(PRIVATE) > " + message
		return who + " to " + dst.name + " > " + message
	}
	return serverwho + "user not found"
}

// Get the local time
func getLocaltime() string {

	location, err := time.LoadLocation("America/Mazatlan")
	if err != nil {
		return serverwho + "Failed loading location. Retry."
	}

	return serverwho + "Local Time: " + location.String() + " " + time.Now().Format("15:04:05")
}

// Get user info
func getUserInfoString(usr *user) string {

	if usr != nil {
		return serverwho + "User: " + usr.name + ", IP: " + usr.ip + " Admin: " +
			strconv.FormatBool(usr.admin)
	}
	return serverwho + "User not found"

}

// Kick user
func kickUser(usr, who *user) string {
	if usr != nil {

		if usr == who {
			return serverwho + "you cannot kick yourself"
		}

		usr.notKicked = false
		usr.ch <- serverwho + "you have been kicked out of the chat by admin"
		leaving <- usr.ch
		messages <- serverwho + who.name + " has kicked out " + usr.name
		return serverwho + "successfuly kicked out"
	}
	return serverwho + "user not found"
}

// pass the baton: if admin leaves make another one admin
func passTheBaton(usr *user) *user {
	if len(users) > 1 {
		if users[0] != usr {
			return users[0]
		}
		return users[1]
	}
	return nil
}

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}
			// Server output
			fmt.Println(msg)

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			deleteUser(cli)
			close(cli)
		}
	}
}

//!-broadcaster

func userNameFromConn(conn net.Conn) string {
	// get username
	buf := make([]byte, 512)
	n, _ := conn.Read(buf)
	return string(buf[:n])
}

func validateUsername(username string) string {
	for _, user := range users {
		if user.name == username {
			s := rand.NewSource(time.Now().UnixNano())
			return validateUsername("username" + strconv.Itoa(rand.New(s).Intn(10000)))
		}
	}
	return username
}

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	// Get the username
	username := userNameFromConn(conn)

	// Validate username or enter a new one
	username = validateUsername(username)

	// Create user
	user := newUser(username, conn.RemoteAddr().String())
	who := user.name
	ch <- serverwho + "Welcome to Simple IRC Server"
	ch <- serverwho + "You are successfuly logged with username: " + who
	messages <- serverwho + who + " has arrived to the chat"
	entering <- ch

	// Append to users list
	user.ch = ch
	users = append(users, user)
	user.notKicked = true

	// Make chat admin the first one connected
	if len(users) == 1 {
		ch <- serverwho + "Since you are the first user you are now ADMIN"
		fmt.Println(serverwho + who + " is ADMIN")
		user.admin = true
	}

	input := bufio.NewScanner(conn)
	for input.Scan() {
		switch tokens := strings.Split(input.Text(), " "); tokens[0] {

		case "/users":
			// get all users from user list
			ch <- getAllUsers()

		case "/msg":
			// message specific user
			if len(tokens) < 3 {
				ch <- serverwho + "usage: /msg <user> <msg>..."
			} else {
				ch <- messageUser(getUserByName(tokens[1]), who, strings.Join(tokens[2:], " "))
			}

		case "/time":
			// get local time
			ch <- getLocaltime()

		case "/user":
			// get user info
			if len(tokens) == 1 {
				ch <- serverwho + "usage: /user <username>"
			} else {
				ch <- getUserInfoString(getUserByName(tokens[1]))
			}

		case "/kick":
			// kick a user (ONLY ADMIN)
			if user.admin {
				// kick user (input[1])
				if len(tokens) == 1 {
					ch <- serverwho + "usage: /kick <username>"
				} else {
					ch <- kickUser(getUserByName(tokens[1]), user)
				}
			} else {
				ch <- serverwho + "only ADMIN can kick users"
			}
		default:
			if user.notKicked {
				messages <- who + " > " + input.Text()
			} else {
				conn.Close()
			}

		}
	}
	// NOTE: ignoring potential errors from input.Err()

	if user.admin {
		if newAdmin := passTheBaton(user); newAdmin != nil {
			newAdmin.admin = true
			messages <- serverwho + newAdmin.name + " is now ADMIN"
		}
	}
	if user.notKicked {
		leaving <- ch
		messages <- serverwho + who + " has left"
	}

	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

//!-handleConn

//!+main
func main() {

	if len(os.Args) < 5 {
		log.Println("Usage: ./client -host localhost -port 9000")
		os.Exit(1)
	}

	var host string
	var port string

	if os.Args[1] == "-host" && os.Args[3] == "-port" {
		host = os.Args[2]
		port = os.Args[4]
	} else if os.Args[1] == "-port" && os.Args[3] == "-host" {
		port = os.Args[2]
		host = os.Args[4]
	} else {
		log.Println("Check your parameters")
		os.Exit(1)
	}

	listener, err := net.Listen("tcp", host+":"+port)
	fmt.Println(serverwho + "Simple IRC Server started at " + host + ":" + port)
	fmt.Println(serverwho + "Ready to receive clients")
	if err != nil {
		log.Fatal(err)
	}

	go broadcaster()
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
