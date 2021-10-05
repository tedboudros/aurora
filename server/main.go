package main

import (
	fiber "github.com/gofiber/fiber/v2"
	"github.com/tedboudros/aurora/server/Integrations"
	"log"
)

func main() {
	app := fiber.New()
	// Future performance improvement: Add this to fiber.New()
	//	fiber.Config{
	//		Prefork: true,
	//	}
	//	to make it multithreaded

	app.Get("/steam", Integrations.HandleSteamGamesGet)

	log.Fatal(app.Listen(":8000"))
}
