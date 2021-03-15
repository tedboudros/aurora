const initSqlJs = require("sql.js");
const fs = require("fs").promises;

const initializeTables = require("./initializeTables");
const getHomeDir = require("../../utils/getHomeDir");

const initialize = async () => {
  let SQL, dbFile, db;

  try {
    SQL = await initSqlJs();
  } catch (e) {
    console.error(e);
  }

  const homeDir = getHomeDir();

  const filePath = `${homeDir}/Documents/.aurora`;

  try {
    dbFile = await fs.readFile(filePath);
  } catch (e) {
    //Every db init function goes here.
    const createTablesStr = initializeTables();

    db = new SQL.Database();
    db.run(createTablesStr);

    //Write db to file
    dbFile = await fs.writeFile(filePath, new Buffer.from(db.export()));
  }

  try {
    if (!db) db = new SQL.Database(dbFile);
  } catch (e) {
    console.error(e);
  }

  return db;
};

module.exports = initialize;
