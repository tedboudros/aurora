package Controllers

import (
	"aurora/server/Controllers/Settings"
	"aurora/server/Controllers/Steam"
	fiber "github.com/gofiber/fiber/v2"
)

func AddRoutes(app *fiber.App) {
	app.Get("/steam", SteamController.HandleSteamGamesGet)
	app.Get("/setting/:setting", SettingsController.GetSetting)
}
