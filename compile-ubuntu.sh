#!/bin/bash
# Compile for target Ubuntu. G++.v

RED_COLOR='\033[0;31m'
GREEN_COLOR='\033[1;32m'
BLUE_COLOR='\033[1;34m'
NO_COLOR='\033[0m' # No Color

error () {
    clear
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

if [[ "$2" == "-v" ]];
  then
    HAS_OUTPUT=""
fi

clear

FOLDER_ENV="build-debug"
ENV="Debug"

if [[ "$1" == "release" ]]; then # Checks first argument: Compilation Environment - Release
    FOLDER_ENV="build-release"
    ENV="Release"
fi

info "Building for ${ENV} env"

# Checking if folder exists
if [ -d "$FOLDER_ENV" ]; then
    eval "cd $FOLDER_ENV && rm -rf * $HAS_OUTPUT"
else
    eval "mkdir $FOLDER_ENV && cd $FOLDER_ENV $HAS_OUTPUT"
fi

eval "cmake .. -DCMAKE_BUILD_TYPE=$ENV $HAS_OUTPUT"
eval "cmake --build . $HAS_OUTPUT"
eval "cmake --install . $HAS_OUTPUT"

success "Built for $ENV env"

for i in "$@" ; do
    if [[ $i == "-zip" ]] ; then
        info "Generating the .zip file"
        eval "cmake --build . --target make_zip $HAS_OUTPUT"
        success "Generated the .zip file"
    elif [[ $i == "-run" ]] ; then
        info "Launching Aurora..."
        cd ..
        eval "./bin/$ENV/Aurora $HAS_OUTPUT"
    fi
done

