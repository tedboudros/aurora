import os

steam_constants = {
    "base_dir": {
        "win32": "C:/Program Files (x86)/Steam",
        "linux": os.path.expanduser("~/.local/share/Steam"),
        "darwin": os.path.expanduser("~/Library/Application Support/Steam")
    },
    "platform_name": "steam",
    "blacklisted_game_ids": [228980]
}