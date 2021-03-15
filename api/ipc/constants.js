const makeType = (type) => {
  return {
    [type]: {
      REQ: `REQ_${type}`,
      RES: `RES_${type}`,
    },
  };
};

module.exports = {
  ...makeType("START_STEAM_GAME"),
  ...makeType("GET_APPS"),
  ...makeType("GET_IS_DEV"),
  ...makeType("QUIT_APP"),
  ...makeType("TOGGLE_FULLSCREEN"),
  ...makeType("CHECK_IF_GAME_IS_RUNNING"),
};
