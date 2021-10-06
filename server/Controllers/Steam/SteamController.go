package SteamController

import (
	SteamIntegration "aurora/server/Integrations/Steam"
	fiber "github.com/gofiber/fiber/v2"
)

func GetSteamGames(c *fiber.Ctx) error {
	allSteamGames := SteamIntegration.GetAllSteamGames()

	return c.JSON(allSteamGames)
}

func LaunchSteamGame(c *fiber.Ctx) error {
	body := struct {
		AppID string `json:"app_id"`
	}{}

	if err := c.BodyParser(&body); err != nil {
		return err
	}

	SteamIntegration.OpenSteamGame(body.AppID)

	return c.SendStatus(200)
}