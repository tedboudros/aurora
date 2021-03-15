const getFromDB = (db, table, condition) => {
  const res = db.exec(
    `SELECT * FROM ${table}${condition ? ` WHERE ${condition}` : ""};`
  );
  const { values, columns } = res.length ? res[0] : { values: [] };

  return values.map((value) => {
    return Object.assign(
      {},
      ...columns.map((column, index) => ({ [column]: value[index] }))
    );
  });
};

const get = (db, table, conditions) => {
  const conditionsStr = conditions
    ? Object.keys(conditions)
        .map((condKey) => `${condKey}=${conditions[condKey]}`)
        .join(" AND ")
    : null;

  return getFromDB(db, table, conditionsStr);
};

module.exports = get;
