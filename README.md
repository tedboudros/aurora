# Aurora

> A potential life-saver for people looking to couch game with their PC.

Aurora is a set of utilities aiming to take PC couch gaming to the next level!

## :books: Table of Contents

- [Installation](#package-installation)
- [Compilation](#rocket-compilation)
- [Support](#hammer_and_wrench-support)
- [Contributing](#memo-contributing)
- [License](#scroll-license)

## :package: Installation

### Windows:

- Install [MSYS2](https://www.msys2.org/)
- Open a new **MSYS2 MinGW 64-bit** terminal (C:\msys64\mingw64.exe)
- Clone the repository
- Update MSYS2 and install dependencies:

```sh
pacman -Syuu && pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf
```

### Linux:
- Clone the repository
- Install dependencies:

```sh
sudo apt install cmake g++ gcc libsdl2-dev libsdl2-2.0-0 libsdl2-image-dev libsdl2-image-2.0-0 libsdl2-ttf-dev libsdl2-ttf-2.0-0
```

- **Fix for Ubuntu 21.04:**

```sh
sudo apt install libudev1=247.3-3ubuntu3.1
```

### Mac OS:

Sorry but you're out of luck...
If you really want to try Aurora try to run it in a virtual machine with Linux!

## :rocket: Compilation

#### To compile (and run) Aurora, you will have to use the `./compile.sh` script.

```sh
cd aurora
./compile.sh <build-env> (args)
```

Build environments:

- `debug`
- `release`

Arguments

- `-v` - Verbose mode (more debugging yay :cry:)
- `-dontRun` - By default, running the compile script will also run Aurora. If you want to stop that behaviour pass this argument.
- `-zip` - This will make a portable **.zip** file of the whole installation.

#### Example:

Let's say you want to compile and run the debug environment with some extra info in case something breaks :boom:

`./compile.sh debug -v`

## :hammer_and_wrench: Support

Please [open an issue](https://github.com/tedboudros/aurora/issues/new) for support.

## :memo: Contributing

Please contribute using [Github Flow](https://guides.github.com/introduction/flow/). Create a branch, add commits, and [open a pull request](https://github.com/tedboudros/aurora/compare/).

## :scroll: License

[GNU GPLv3](LICENSE) © [tedboudros](https://github.com/tedboudros)
