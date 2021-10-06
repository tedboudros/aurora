package SteamController

import (
	"aurora/server/Integrations/Steam"
	fiber "github.com/gofiber/fiber/v2"
)

func HandleSteamGamesGet(c *fiber.Ctx) error {
	allSteamGames := SteamIntegration.GetAllSteamGames()

	return c.JSON(allSteamGames)
}
