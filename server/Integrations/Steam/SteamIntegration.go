package SteamIntegration

import (
	"errors"
	"github.com/andygrunwald/vdf"
    "github.com/pkg/browser"
	"log"
	"os"
	"path/filepath"
	"strconv"
)

type formattedGame struct {
	Name         string
	AppDirectory string
	Language     string
	Size         int
	SteamAppID   string
}

func ReadVDFFile(filename string) map[string]interface{} {
	file, err := os.Open(filename)

	if err != nil {
		log.Fatal("Error reading VDF file:", err)
	}

	parser := vdf.NewParser(file)
	contents, err := parser.Parse()

	if err != nil {
		log.Fatal("Error parsing VDF file:", err)
	}

	return contents
}

func GetSteamLibraries(baseLibrary string) []string {

	libraryFolderFile := baseLibrary + "\\libraryfolders.vdf"
	libraryFoldersFromVDF := ReadVDFFile(libraryFolderFile)["libraryfolders"].(map[string]interface{})

	var libraryFolders []string
	libraryFolders = append(libraryFolders, baseLibrary)

	for key, libraryFolderInterface := range libraryFoldersFromVDF {
		if key != "contentstatsid" {
			libraryFolder, _ := libraryFolderInterface.(map[string]interface{})
			libraryFolders = append(libraryFolders, libraryFolder["path"].(string)+"/steamapps")
		}
	}

	return libraryFolders

}

func GetSteamGameVDFsFromLibrary(libraryDir string) []string {
	gameVDFs, err := filepath.Glob(libraryDir + "/*.acf")

	if err != nil {
		log.Fatal("Error globbing files from Steam library dir: "+libraryDir, err)
		return []string{}
	}

	return gameVDFs

}

func FormatGameFromVDF(libraryFolder string, vdf map[string]interface{}) (formattedGame, error) {

	var game formattedGame

	appState := vdf["AppState"].(map[string]interface{})

	nameInterface := appState["name"]
	sizeOnDiskInterface := appState["SizeOnDisk"]
	appDirectoryInterface := appState["installdir"]
	steamAppIDInterface := appState["appid"]
	userConfigInterface := appState["UserConfig"]

	if nameInterface != nil && sizeOnDiskInterface != nil && appDirectoryInterface != nil && steamAppIDInterface != nil && userConfigInterface != nil {
		name := nameInterface.(string)
		sizeOnDisk, _ := strconv.Atoi(sizeOnDiskInterface.(string))
		appDirectory := libraryFolder + "/common/" + appDirectoryInterface.(string)
		steamAppID := steamAppIDInterface.(string)

		languageInterface := userConfigInterface.(map[string]interface{})["language"]
		if languageInterface == nil {
			return game, errors.New("couldn't find language inside UserConfig")
		}

		language := languageInterface.(string)

		game = formattedGame{
			Name:         name,
			AppDirectory: appDirectory,
			Size:         sizeOnDisk,
			SteamAppID:   steamAppID,
			Language:     language,
		}

		return game, nil
	}

	return game, errors.New("error reading vdf")
}

func GetAllSteamGames() []formattedGame {
	const baseLibrary string = "C:\\Program Files (x86)\\Steam\\steamapps"
	libraryFolders := GetSteamLibraries(baseLibrary)

	var formattedGames []formattedGame

	for _, libraryFolder := range libraryFolders {
		gamesFromLibrary := GetSteamGameVDFsFromLibrary(libraryFolder)

		for _, gameFilepath := range gamesFromLibrary {
			vdf := ReadVDFFile(gameFilepath)
			newFormattedGame, err := FormatGameFromVDF(libraryFolder, vdf)

			if err == nil {
				formattedGames = append(formattedGames, newFormattedGame)
			}
		}
	}

	return formattedGames
}


func OpenSteamGame(appID string) {
	browser.OpenURL("steam://rungameid/" + appID)
}