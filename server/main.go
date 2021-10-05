package main

import (
	fiber "github.com/gofiber/fiber/v2"
	"github.com/tedboudros/aurora/server/Integrations"
	"log"

	//"fmt"
)

func main() {
	app := fiber.New(fiber.Config{
		Prefork: true,
	})

	app.Get("/steam", Integrations.HandleSteamGamesGet)

	log.Fatal(app.Listen(":8000"))
}
