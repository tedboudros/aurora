const fs = require("fs").promises;

const getHomeDir = require("../utils/getHomeDir");

const saveDB = async (db) => {
  const homeDir = getHomeDir();
  const filePath = `${homeDir}/Documents/.aurora`;

  const data = db.export();
  const buffer = new Buffer.from(data);
  await fs.writeFile(filePath, buffer);
};

module.exports = saveDB;
