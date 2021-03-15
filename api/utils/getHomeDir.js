const os = require("os");

const getHomeDir = () => {
  return os.homedir();
};

module.exports = getHomeDir;
