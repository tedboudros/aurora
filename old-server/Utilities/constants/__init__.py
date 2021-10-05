import sys

from Utilities.constants.steam_constants import steam_constants
from Utilities.logger import logger

constants = {
    "steam": steam_constants
}


def get_constant(group, constant_name):
    constant = constants[group][constant_name]

    if isinstance(constant, object) and not isinstance(constant, str):
        # In this case, we choose the constant depending on the os that this is running on
        if not sys.platform in constant:
            logger.error(f"Constant '{constant_name}' is not supported by your OS")
            return None
        return constant[sys.platform]

    return constant
