package Database

import (
	"aurora/server/Database/Models"
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/sqlite"
	"log"
)

var (
	DBConn *gorm.DB
)

func runMigrations() {
	log.Println("Running auto-migrations...")
	DBConn.AutoMigrate(&Models.Setting{})
}

func InitDB() {
	var err error
	DBConn, err = gorm.Open("sqlite3", "aurora.db")
	if err != nil {
		log.Fatal("Failed to connect to database")
	}
	log.Println("Successfully connected to database")

	runMigrations()
}
