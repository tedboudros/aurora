package main

import (
	"aurora/server/Controllers"
	"aurora/server/Database"
	fiber "github.com/gofiber/fiber/v2"
	"log"
)

func main() {
	app := fiber.New(fiber.Config{
		ServerHeader: "Aurora",
		AppName:      "Aurora Server",
	})

	Database.InitDB()
	defer Database.DBConn.Close()

	Controllers.AddRoutes(app)

	log.Fatal(app.Listen(":8000"))
}
