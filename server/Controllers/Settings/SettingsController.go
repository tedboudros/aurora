package SettingsController

import (
	"aurora/server/Database"
	"aurora/server/Database/Models"
	fiber "github.com/gofiber/fiber/v2"
)

func GetSetting(c *fiber.Ctx) error {
	settingLabel := c.Params("setting")
	db := Database.DBConn

	var setting Models.Setting

	db.Where("label = ?", settingLabel).First(&setting)

	if setting.ID == 0 {
		return c.SendStatus(404)
	}

	return c.JSON(setting.Value)
}
