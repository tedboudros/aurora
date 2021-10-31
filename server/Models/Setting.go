package Models

import "github.com/jinzhu/gorm"

type Setting struct {
	gorm.Model
	Label string `json:"label"`
	Value string `json:"value"`
}
