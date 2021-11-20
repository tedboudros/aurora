package main

import (
	"log"
	"os"
	"os/exec"
	"strconv"
)

func openServer(port int) int {
	log.Println("Starting server...")
	server := exec.Command("server.exe", strconv.Itoa(port))
	server.Stdout = os.Stdout
	err := server.Start()
	if err != nil {
		log.Fatal(err)
		return 0 // 0 if something fails
	}

	return server.Process.Pid
}

func openClient(port int) int {
	log.Println("Starting client...")
	server := exec.Command("client.exe", strconv.Itoa(port))
	server.Stdout = os.Stdout
	err := server.Start()
	if err != nil {
		log.Fatal(err)
		return 0 // 0 if something fails
	}

	return server.Process.Pid
}

func closeClient(port int) bool {

	return false // Returns false if it closed successfully
}

func main() {
	const TcpPort int = 8000

	//clientPID := openClient(TcpPort)
	openServer(TcpPort)
	openClient(TcpPort)
	//closeClient(clientPID)

}
