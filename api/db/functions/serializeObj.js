const _isString = require("lodash/isString");

const serializeObj = (obj) => {
  const keys = Object.keys(obj);

  return Object.assign(
    ...keys.map((key) => {
      const data = obj[key];

      if (_isString(data)) return { [key]: `"${data}"` };
      return { [key]: data };
    })
  );
};

module.exports = serializeObj;
