# Aurora

> A potential life-saver for people looking to couch game with their PC.

Aurora is a set of utilities aiming to take PC couch gaming to the next level!

## :books: Table of Contents

- [Installation](#package-installation) | [Compilation](#rocket-compilation) | [Support](#hammer_and_wrench-support) | [Contributing](#memo-contributing) | [License](#scroll-license)

## :package: Installation

### Windows:

- Install [MSYS2](https://www.msys2.org/) as well as [Python 3.6+](https://www.python.org/downloads/)
- Clone the repository - Inside a MSYS2 MinGW64 terminal
- Update MSYS2 and install dependencies - Inside a MSYS2 MinGW64 terminal

```sh
pacman -Syuu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf
```

- Install python dependencies - Inside a PowerShell terminal

```sh
cd launcher
pip3 install -r requirements.txt
cd ../server
pip3 install -r requirements.txt
```

---

### Linux:

- Clone the repository
- Install dependencies:

```sh
sudo apt install python3 python3-pip cmake g++ gcc libsdl2-dev libsdl2-2.0-0 libsdl2-image-dev libsdl2-image-2.0-0 libsdl2-ttf-dev libsdl2-ttf-2.0-0
```

- Install python dependencies

```sh
cd launcher
pip3 install -r requirements.txt
cd ../server
pip3 install -r requirements.txt
```

---

- **Fix for Ubuntu 21.04:**

```sh
sudo apt install libudev1=247.3-3ubuntu3.1
```

---

### Mac OS:

#### You are going to need to have [Homebrew](https://brew.sh/) installed

- Install dependencies

```sh
brew install python3 sdl2 sdl2_image sdl2_ttf
```

- Clone the repository
- Install python dependencies

```sh
cd launcher
pip3 install -r requirements.txt
cd ../server
pip3 install -r requirements.txt
```

---

## :rocket: Development

**MacOS** | **Linux**

```sh
./develop.sh <args>
```

**Windows** (PowerShell or Command Prompt):

```sh
./develop.bat <args>
```

---

#### Both of those are just an alias to

```sh
python3 launcher/development.py <args>
```

---

### Script Arguments:

| Argument        | Short Argument | Description                                                                                                             |
| --------------- | -------------- | ----------------------------------------------------------------------------------------------------------------------- |
| --verbose       | -v             | Verbose mode. Provides more debugging logs from mostly **cmake**                                                        |
| --port **PORT** | -p             | The TCP port used for the internal server/client communication **(Default: 8000)**                                      |
| --env **ENV**   | -e             | The environment to compile for. Mostly used for the client. Options: **"debug"** or **"release"**. **(Default: debug)** |

## :hammer_and_wrench: Support

Please [open an issue](https://github.com/tedboudros/aurora/issues/new) for support.

## :memo: Contributing

Please contribute using [Github Flow](https://guides.github.com/introduction/flow/). Create a branch, add commits, and [open a pull request](https://github.com/tedboudros/aurora/compare/).

## :scroll: License

[GNU GPLv3](LICENSE) © [tedboudros](https://github.com/tedboudros)
