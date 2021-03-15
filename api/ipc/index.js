const games = require("./games");
const isDev = require("./isDev");

module.exports = (win, db) => {
  games(db, win);
  isDev(win);
};
