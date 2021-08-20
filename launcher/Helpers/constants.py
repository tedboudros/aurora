import sys
import os

CLIENT_DIR="client"
SERVER_DIR="server"

TIME_TO_ELAPSE_FROM_LAST_CLIENT_COMPILATION = 5 # in seconds
TIME_TO_ELAPSE_FROM_LAST_SERVER_COMPILATION = 5 # in seconds

DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION = ["src\\", "include\\", "src/", "include/"]

arguments_default_values = {
    "port": 8000,
    "env": 'debug',
    'verbose': False
}

def get_msys2_mingw64_terminal():
    dir = os.getcwd()
    return f"c:\\msys64\\usr\\bin\\env MSYSTEM=MINGW64 HOME=\"{dir}\" c:\\msys64\\usr\\bin\\bash -l -c"


def get_cmake_win_cmd(cmd):
    msys2_mingw64_terminal = get_msys2_mingw64_terminal()
    return f"{msys2_mingw64_terminal} \'cmake {cmd}\'"


def get_arg_index(arg_name):
    index = -1

    for i, arg in enumerate(sys.argv):
        mini_arg_str = f"-{arg_name[0]}"
        full_arg_str = f"--{arg_name}"

        if arg == mini_arg_str or arg == full_arg_str:
            index = i
            continue

    return index


def get_arg_value(arg_name):
    arg_index = get_arg_index(arg_name)

    if arg_index == -1:
        return arguments_default_values[arg_name]

    return sys.argv[arg_index+1]


def get_arg_exist(arg_name):
    return get_arg_index(arg_name) != -1


AURORA_ASCII_ART=r"""
__________________________________________

     /\                                  
    /  \   _   _  _ __  ___   _ __  __ _ 
   / /\ \ | | | || '__|/ _ \ | '__|/ _` |
  / ____ \| |_| || |  | (_) || |  | (_| |
 /_/    \_\\__,_||_|   \___/ |_|   \__,_|

__________________________________________
"""