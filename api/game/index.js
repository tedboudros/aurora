const fs = require("fs").promises;
const { exec } = require("child_process");
const { parse } = require("@node-steam/vdf");
const flatten = require("lodash/flatten");
const find = require("find-process");
const path = require("path");

const searchForFile = require("../utils/searchFile");
const filterGameFiles = require("../utils/filterGameFiles");

const steam = require("./steamConstants");

const isNumeric = (str) => {
  if (typeof str != "string") return false;
  return !isNaN(str) && !isNaN(parseFloat(str));
};

const getSteamLibraryDirectories = () =>
  new Promise(async (resolve) => {
    const librariesFile = await fs.readFile(
      `${steam.baseDir}/steamapps/libraryfolders.vdf`,
      steam.encoding
    );
    const parsedFile = parse(librariesFile)["LibraryFolders"];

    const libraryFolders = [
      steam.baseDir,
      ...Object.keys(parsedFile)
        .filter((key) => isNumeric(key))
        .map((key) => parsedFile[key]),
    ].map((dir) => `${dir}/steamapps/`);

    resolve(libraryFolders);
  });

const getSteamGamesListFromLibrary = async (dir) => {
  const dirList = await fs.readdir(dir);
  const gameFiles = dirList.filter((file) => file.includes(".acf"));

  return gameFiles
    .map(async (gameFile) => {
      const gameACFPath = `${dir}/${gameFile}`;

      let parsedFile = {};

      try {
        const file = await fs.readFile(gameACFPath, steam.encoding);
        parsedFile = parse(file)["AppState"];
      } catch (e) {
        console.error(e);
        return null;
      }

      const gameDirectory = `${dir}/common/${parsedFile.installdir}`;

      let executables = [];

      try {
        executables = await searchForFile(`${gameDirectory}/**/*.exe`);
      } catch (e) {
        console.error(e);
      }

      const filteredExecutables = filterGameFiles(executables);

      const finalGame = {
        appDir: gameDirectory,
        executables: filteredExecutables,
        platform: steam.platform,
        language: parsedFile.UserConfig.language,
        size: parsedFile.SizeOnDisk,
        steamAppID: parsedFile.appid,
        name: parsedFile.name,
      };

      if (!steam.blacklistIds.includes(parsedFile.appid)) return finalGame;
      return null;
    })
    .filter((t) => t);
};

const getSteamGamesList = async () => {
  const libraryFolders = await getSteamLibraryDirectories();

  console.log(`Steam library folders detected: [${libraryFolders.join(", ")}]`);

  const mappedGamesFromLibraries = await Promise.all(
    libraryFolders.map(async (dir) => await getSteamGamesListFromLibrary(dir))
  );

  const games = await Promise.all(flatten(mappedGamesFromLibraries));

  console.log(`Games found: ${games.length}`);

  return games;
};

const startSteamGame = (appId) => {
  const command = `"${steam.baseDir}/steam.exe" -applaunch ${appId}`;

  console.log(`Launching steam game with appId: ${appId}`);
  exec(command, (error, stdout, stderr) => {
    if (error) {
      console.error(`exec error: ${error}`);
      return;
    }
    if (stdout) console.log(`stdout: ${stdout}`);
    if (stderr) console.error(`stderr: ${stderr}`);
  });
};

const checkIfGameIsRunning = async (executables) => {
  const processesArray = await Promise.all(
    executables.map(async (gameExecutable) => {
      const { name } = gameExecutable;
      console.log(`Searching for file: ${name}`);

      return await find("name", name, false);
    })
  );

  const processes = flatten(processesArray);

  return processes.length > 0;
};

module.exports = {
  startSteamGame,
  getSteamLibraryDirectories,
  getSteamGamesList,
  checkIfGameIsRunning,
};
