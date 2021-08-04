import glob
import vdf

from Utilities.constants import get_constant
from Utilities.logger import logger


def get_all_steam_libraries_fs():
    steam_base_dir = get_constant('steam', 'base_dir')
    logger.info(f"Trying to find all steam libraries from the filesystem...")

    steam_libraries = [f"{steam_base_dir}/steamapps/"]
    extra_steam_libraries_vdf = vdf.load(open(f"{steam_base_dir}/steamapps/libraryfolders.vdf"))

    for key, extra_steam_library in extra_steam_libraries_vdf['LibraryFolders'].items():
        if key.isnumeric():  # Steam libraries inside the vdf will have numeric only keys
            steam_libraries.append(f"{extra_steam_library}/steamapps/")

    logger.info(f"Found in total {len(steam_libraries)} steam libraries!")
    return steam_libraries


def format_steam_game_from_vdf(library_dir, game):
    formatted_game = {
        'app_directory': f"{library_dir}/common/{game['installdir']}",
        'platform': get_constant('steam', 'platform_name'),
        "language": game["UserConfig"]["language"],
        "size": game["SizeOnDisk"],
        "steam_app_id": game["appid"],
        "name": game["name"],
    }

    return formatted_game


def get_steam_games_from_dir_fs(library_dir):
    games = []

    for file in glob.glob(f"{library_dir}/*.acf"):
        game_vdf = None
        try:
            game_vdf = vdf.load(open(file))['AppState']
        except SyntaxError:
            logger.error(f"Failed to load {file} file. The file is very likely corrupted.")

        if game_vdf:
            try:
                games.append(format_steam_game_from_vdf(library_dir, game_vdf))
            except KeyError:
                logger.warn(f"Couldn't format '{file}'. If the file is appmanifest_228980.acf, completely ignore "
                            f"this. If not, a value might be missing from the .acf file")

    return games


def get_all_steam_games_from_filesystem():
    all_games = []
    steam_libraries = get_all_steam_libraries_fs()

    for library_dir in steam_libraries:
        games = get_steam_games_from_dir_fs(library_dir)
        logger.info(f"Successfully read {len(games)} steam games from '{library_dir}'")
        all_games = all_games + games

    return all_games
