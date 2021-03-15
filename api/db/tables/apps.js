const apps = {
  id: "INTEGER PRIMARY KEY AUTOINCREMENT",
  steamAppID: "INTEGER",
  name: "CHAR",
  platform: "CHAR",
  appDir: "CHAR",
  language: "CHAR",
  size: "INTEGER",
};

module.exports = apps;
