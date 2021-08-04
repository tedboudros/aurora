#!/bin/bash
# Tested with (Ubuntu 21.04 w/ G++, Windows 10 w/ MSYS MinGW64)

RED_COLOR='\033[0;31m'
GREEN_COLOR='\033[1;32m'
BLUE_COLOR='\033[1;34m'
NO_COLOR='\033[0m' # No Color

error () {
    echo -e "🚫 ${RED_COLOR}ERROR - $1!${NO_COLOR}"
}

info () {
    echo -e "👋 ${BLUE_COLOR}INFO - $1...${NO_COLOR}"
}

success () {
    echo -e "👌 ${GREEN_COLOR}SUCCESSFULLY - $1!${NO_COLOR}"
    echo -e "\n"
}

die () {
    error "$1"
    exit 1
}

if [ $# -eq 0 ]; then
    die "No compilation environment supplied"
fi

if [[ "$1" != "debug" ]] && [[ "$1" != "release" ]];
  then
    die "Wrong compilation environment supplied. Correct values: (${NO_COLOR}debug${RED_COLOR} || ${NO_COLOR}release${RED_COLOR})"
fi

HAS_OUTPUT="&> /dev/null"

if [[ $@ == *'-v'* ]];
  then
    HAS_OUTPUT=""
fi

#clear

CLIENT_FOLDER_ENV="client/build-debug"
ENV="Debug"

if [[ "$1" == "release" ]]; then # Checks first argument: Compilation Environment - Release
    CLIENT_FOLDER_ENV="client/build-release"
    ENV="Release"
fi

info "Building for ${ENV} env"

# Checking if folder exists
if [ -d "$CLIENT_FOLDER_ENV" ]; then
    eval "cd $CLIENT_FOLDER_ENV && rm -rf * $HAS_OUTPUT"
else
    eval "mkdir $CLIENT_FOLDER_ENV && cd $CLIENT_FOLDER_ENV $HAS_OUTPUT"
fi

BUILD_EXTRA_PARAMS=""

if [[ "$OSTYPE" == "msys" ]]; then
    BUILD_EXTRA_PARAMS=' -G "MinGW Makefiles"'
fi

eval "cmake$BUILD_EXTRA_PARAMS .. -DCMAKE_BUILD_TYPE=$ENV $HAS_OUTPUT"
eval "cmake --build . $HAS_OUTPUT"
eval "cmake --install . $HAS_OUTPUT"

if [ -f "Aurora.exe" ] || [ -f "Aurora" ]; then
  success "Built for $ENV env"
else
  die " Build failed. Please run the command with -v to see more details"
fi

if [[ $@ == *'-zip'* ]] ; then
    info "Generating the .zip file"
    eval "cmake --build . --target make_zip $HAS_OUTPUT"
    success "Generated the .zip file"
fi


SHOULD_RUN=true

if [[ $@ == *'-dontRun'* ]] ; then
    SHOULD_RUN=false
fi

if [ $SHOULD_RUN == true ]; then
    info "Launching Aurora..."
    cd ..
    eval "./bin/$ENV/Aurora $HAS_OUTPUT"
fi
