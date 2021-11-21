#!/bin/bash

# Personal variables (this is what to fiddle with if you're having issues)
BUILD_MODE="debug"
MSYS2_EXECUTABLE="C:\\msys64\\mingw64.exe"

# Internal variables (don't mess with)
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
CLIENT_COMPILE_SCRIPT="compile-client.sh"
CLIENT_EXECUTABLE_NAME="aurora_client"
SERVER_EXECUTABLE_NAME="aurora_server"
LAUNCHER_EXECUTABLE_NAME="launcher"
LOG_PREFIX="-- BUILD | "

# Colors
PURPLE_COLOR='\033[0;35m'
GREEN_COLOR='\033[0;32m'
BLUE_COLOR='\033[1;34m'
NO_COLOR='\033[0m' # No Color


success () {
  echo -e "${GREEN_COLOR}${LOG_PREFIX}SUCCESS - $1!${NO_COLOR}"
}

info () {
  echo -e "${BLUE_COLOR}${LOG_PREFIX}INFO - $1${NO_COLOR}"
}

announcement () {
  echo -e ""
  echo -e "${PURPLE_COLOR}${LOG_PREFIX}$1${NO_COLOR}"
}


# CLIENT:
announcement "Starting client build"

# Make build dir:
info "Purging and remaking build directory"
rm -rf "${SCRIPT_DIR}/build" &> /dev/null
mkdir "${SCRIPT_DIR}/build" &> /dev/null

# Compile client
info "Starting compilation of client"
cd "${SCRIPT_DIR}/client"
bash ${CLIENT_COMPILE_SCRIPT} ${BUILD_MODE} --dontRun

# Move client inside build dir
info "Moving client to build directory"
mv -v "${SCRIPT_DIR}"/client/bin/Debug/* "${SCRIPT_DIR}/build" &> /dev/null

# Renaming executable so that the launcher can work
info "Renaming client executable for launcher"
if [[ "$OSTYPE" == "msys" ]]; then
  mv -v "${SCRIPT_DIR}/build/Aurora.exe" "${SCRIPT_DIR}/build/${CLIENT_EXECUTABLE_NAME}.exe" &> /dev/null
else
  mv -v "${SCRIPT_DIR}/build/Aurora" "${SCRIPT_DIR}/build/${CLIENT_EXECUTABLE_NAME}" &> /dev/null
fi

# Client cleanup
info "Cleaning up temporary client directory"
rm -rf "${SCRIPT_DIR}/client/bin" &> /dev/null
rm -rf "${SCRIPT_DIR}/client/build-debug" &> /dev/null


# SERVER:
announcement "Starting server build"

# Compile server
info "Starting compilation of server"
cd "${SCRIPT_DIR}/server"
export CGO_CFLAGS="-g -O2 -Wno-return-local-addr" # Fix for a weird bug with a go package
go build -o ${SERVER_EXECUTABLE_NAME} main.go
success "Built server"

info "Moving server to build directory"
if [[ "$OSTYPE" == "msys" ]]; then
  mv "${SCRIPT_DIR}/server/${SERVER_EXECUTABLE_NAME}" "${SCRIPT_DIR}/build/${SERVER_EXECUTABLE_NAME}.exe" &> /dev/null
else
  mv "${SCRIPT_DIR}/server/${SERVER_EXECUTABLE_NAME}" "${SCRIPT_DIR}/build/${SERVER_EXECUTABLE_NAME}" &> /dev/null
fi


# LAUNCHER:
announcement "Starting launcher build"

# Compile server
info "Starting compilation of launcher"
cd "${SCRIPT_DIR}/launcher"
export CGO_CFLAGS="-g -O2 -Wno-return-local-addr" # Fix for a weird bug with a go package
go build -o ${LAUNCHER_EXECUTABLE_NAME} main.go
success "Built launcher"

info "Moving launcher to build directory"
if [[ "$OSTYPE" == "msys" ]]; then
  mv "${SCRIPT_DIR}/launcher/${LAUNCHER_EXECUTABLE_NAME}" "${SCRIPT_DIR}/build/${LAUNCHER_EXECUTABLE_NAME}.exe" &> /dev/null
else
  mv "${SCRIPT_DIR}/launcher/${LAUNCHER_EXECUTABLE_NAME}" "${SCRIPT_DIR}/build/${LAUNCHER_EXECUTABLE_NAME}" &> /dev/null
fi