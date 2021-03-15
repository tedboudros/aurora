const glob = require("glob");

const searchForFile = (pattern) =>
  new Promise((resolve) => {
    glob(pattern, function (er, files) {
      if (er) {
        console.error(er);
        resolve([]);
      }
      resolve(files);
    });
  });

module.exports = searchForFile;
