package main

import (
	"log"
	"os"
	"os/exec"
	"runtime"
	"strconv"
)

func getExecutableName(filename string) string {
	switch runtime.GOOS {
	case "windows":
		return filename + ".exe"
	case "linux":
		return filename
	case "darwin":
		return filename
	}

	return ""
}

func getServerExecutableName() string {
	const executableName string = "aurora_server"
	return getExecutableName(executableName)
}

func getClientExecutableName() string {
	const executableName string = "aurora_client"
	return getExecutableName(executableName)
}

func openServer(port int) *exec.Cmd {
	log.Println("Starting server...")
	server := exec.Command(getServerExecutableName(), strconv.Itoa(port))
	server.Stdout = os.Stdout
	err := server.Start()
	if err != nil {
		log.Fatal(err)
	}

	return server
}

func openClient(port int) *exec.Cmd {
	log.Println("Starting client...")
	client := exec.Command(getClientExecutableName(), strconv.Itoa(port))
	client.Stdout = os.Stdout
	err := client.Start()
	if err != nil {
		log.Fatal(err)
	}

	return client
}

func closeServer(server *exec.Cmd) {
	server.Process.Kill()
}


func main() {
	const TcpPort int = 8000

	server := openServer(TcpPort)
	client := openClient(TcpPort)

	client.Wait()
	closeServer(server)
}
