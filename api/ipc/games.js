const { ipcMain } = require("electron");
const {
  getSteamGamesList,
  startSteamGame,
  checkIfGameIsRunning,
} = require("../game/index");

const _flatten = require("lodash/flatten");
const ipcTypes = require("./constants");
const path = require("path");
const dbFunctions = require("../db/functions");
const saveDB = require("../db/save");

const filterOutBadGames = (games) => {
  return games.filter((game) => (!game ? false : Object.keys(game).length));
};

module.exports = (db, win) => {
  ipcMain.on(ipcTypes.START_STEAM_GAME.REQ, (_, arg) => {
    startSteamGame(arg);
    win.blur();
  });

  ipcMain.on(ipcTypes.GET_APPS.REQ, (event) => {
    const dbGames = dbFunctions.get(db, "apps");

    if (dbGames.length) {
      event.reply(ipcTypes.GET_APPS.RES, dbGames);
      return;
    }

    getSteamGamesList().then((gameList) => {
      const filteredGames = filterOutBadGames(gameList);

      const executables = _flatten(
        filteredGames.map((game, i) =>
          game.executables.map((executable) => ({
            name: path.basename(executable),
            location: executable,
            appID: i + 1,
          }))
        )
      );

      dbFunctions.add(db, "apps", filteredGames);
      dbFunctions.add(db, "executables", executables);

      saveDB(db);

      event.reply(ipcTypes.GET_APPS.RES, dbFunctions.get(db, "apps"));
    });
  });

  ipcMain.on(ipcTypes.CHECK_IF_GAME_IS_RUNNING.REQ, (event, id) => {
    const executables = dbFunctions.get(db, "executables", { appID: id });

    checkIfGameIsRunning(executables).then((isRunning) =>
      event.reply(ipcTypes.CHECK_IF_GAME_IS_RUNNING.RES, isRunning)
    );
  });
};
