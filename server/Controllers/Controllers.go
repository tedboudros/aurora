package Controllers

import (
	"aurora/server/Controllers/Settings"
	"aurora/server/Controllers/Steam"
	fiber "github.com/gofiber/fiber/v2"
)

func AddRoutes(app *fiber.App) {
	app.Get("/steam", SteamController.GetSteamGames)
	app.Post("/steam", SteamController.LaunchSteamGame)

	app.Get("/setting/:setting", SettingsController.GetSetting)
}
