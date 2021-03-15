const path = require("path");

const keywords = [
  "install",
  "setup",
  "directx",
  "unitycrashhandler",
  "unrealcefsubprocess",
];

const filterGameFiles = (files) => {
  return files.filter((filePath) => {
    const file = path.basename(filePath);

    const lowercaseFile = file.toLowerCase();

    return !keywords.some((keyword) => lowercaseFile.includes(keyword));
  });
};

module.exports = filterGameFiles;
