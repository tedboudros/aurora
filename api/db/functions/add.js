const tables = require("../tables");
const serializeObj = require("./serializeObj");

const _isArray = require("lodash/isArray");

const getSingleAddStatement = (table, item = {}) => {
  const columns = Object.keys(tables[table]);
  const serializedItem = serializeObj(item);
  const values = columns.map((col) => serializedItem[col] || "null").join(", ");
  const statement = `INSERT INTO ${table} VALUES (${values});`;

  return statement;
};

const add = (db, table, items) => {
  if (_isArray(items)) {
    const allStatements = items
      .map((item) => getSingleAddStatement(table, item))
      .join(" ");
    db.run(allStatements);

    return;
  }

  const singleStatement = getSingleAddStatement(table, items);
  db.run(singleStatement);

  return;
};

module.exports = add;
