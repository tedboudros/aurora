package main

import (
	"log"
	//"os"
	"os/exec"
	"runtime"
	"strconv"
)

func getExecutableName(filename string) string {
	switch runtime.GOOS {
	case "windows":
		return "./" + filename + ".exe"
	case "linux":
		return "./" + filename
	case "darwin":
		return "./" + filename
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

func openExecutable(port int, filename string, displayName string) *exec.Cmd {
	log.Println("Starting " + displayName + "...")
	executable := exec.Command(filename, strconv.Itoa(port))
	err := executable.Start()
	if err != nil {
		log.Fatal(err)
	}else {
		log.Println("Successfully started "+displayName+"...")
	}

	return executable
}

func closeServer(server *exec.Cmd) {
	server.Process.Kill()
}


func main() {
	const TcpPort int = 32815

	server := openExecutable(TcpPort, getServerExecutableName(), "server")
	client := openExecutable(TcpPort, getClientExecutableName(), "client")

	client.Wait()
	closeServer(server)
}
