import vdf
from Utilities.constants import get_constant
from Utilities.logger import logger


def get_all_steam_libraries_fs():
    steam_base_dir = get_constant('steam', 'base_dir')
    logger.info(f"Trying to find all steam libraries from the filesystem...")

    steam_libraries = [f"{steam_base_dir}/steamapps/"]
    extra_steam_libraries_vdf = vdf.load(open(f"{steam_base_dir}/steamapps/libraryfolders.vdf"))

    for key, extra_steam_library in extra_steam_libraries_vdf['LibraryFolders'].items():
        if key.isnumeric(): # Steam libraries inside the vdf will have numeric only keys
            steam_libraries.append(f"{extra_steam_library}/steamapps/")

    logger.info(f"Found in total {len(steam_libraries)} steam libraries!")
    return steam_libraries