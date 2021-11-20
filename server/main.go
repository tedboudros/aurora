package main

import (
	"aurora/server/Controllers"
	"aurora/server/Database"
	fiber "github.com/gofiber/fiber/v2"
	"log"
	"os"
)

func main() {
	app := fiber.New(fiber.Config{
		ServerHeader: "Aurora",
		AppName:      "Aurora Server",
	})

	Database.InitDB()
	defer Database.DBConn.Close()

	Controllers.AddRoutes(app)

	port := "40096"

	pPort := os.Args[1:]

	if len(pPort) > 0 {
		port = pPort[0]
	}

	log.Fatal(app.Listen(":" + string(port)))
}
