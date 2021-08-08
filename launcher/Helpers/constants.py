import sys

CLIENT_DIR="client"
SERVER_DIR="server"

MSYS2_MINGW64_EXECUTABLE = "C:\\msys64\\mingw64.exe" # windows (msys2) specific
TIME_TO_WAIT_IF_IT_DIDNT_FIND_CMAKE_TASK = 0.2 # in seconds / windows (msys2) specific

TIME_TO_ELAPSE_FROM_LAST_CLIENT_COMPILATION = 5 # in seconds
TIME_TO_ELAPSE_FROM_LAST_SERVER_COMPILATION = 5 # in seconds

DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION = ["src\\", "include\\", "src/", "include/"]

arguments_default_values = {
    "port": 8000,
    "env": 'debug',
    'verbose': False
}

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