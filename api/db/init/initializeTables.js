const tables = require("../tables");

const initializeTables = () => {
  const tableNames = Object.keys(tables);

  return tableNames
    .map((tableName) => {
      const table = tables[tableName];
      const tableColNames = Object.keys(table);
      const tableColString = tableColNames
        .map((colName) => `${colName} ${table[colName]}`)
        .join(", ");

      return `CREATE TABLE ${tableName}(${tableColString});`;
    })
    .join(" ");
};

module.exports = initializeTables;
