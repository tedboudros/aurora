CLIENT_DIR="client"
SERVER_DIR="server"

MSYS2_MINGW64_EXECUTABLE = "C:\\msys64\\mingw64.exe" # windows (msys2) specific
TIME_TO_WAIT_IF_IT_DIDNT_FIND_CMAKE_TASK = 0.2 # in seconds / windows (msys2) specific

TIME_TO_ELAPSE_FROM_LAST_CLIENT_COMPILATION = 5 # in seconds
TIME_TO_ELAPSE_FROM_LAST_SERVER_COMPILATION = 5 # in seconds

DIRECTORIES_TO_WATCH_FOR_CLIENT_RECOMPILATION = ["src\\", "include\\", "src/", "include/"]