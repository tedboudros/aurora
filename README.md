# Aurora

Aurora is a set of utilities aiming to take PC couch gaming to the next level!

## :books: Table of Contents

[Installation](#package-installation) | [Compilation](#rocket-compilation) | [Support](#hammer_and_wrench-support)
| [Contributing](#memo-contributing) | [License](#scroll-license)

## :package: Prerequisites

### Windows:

- Install [MSYS2](https://www.msys2.org/) as well as [Go](https://golang.org/)
- Update MSYS2 and install dependencies - Inside a MSYS2 MinGW64 terminal:

```shell
pacman -Syuu
pacman -S mingw-w64-x86_64-go mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_ttf
```
And then add these 2 lines into your `~/.bashrc`
```shell
export GOROOT=/mingw64/lib/go
export GOPATH=/mingw64
```

### Debian:

- Install [Go](https://golang.org/) by following the instructions shown [here](https://golang.org/doc/install)
- Install other dependencies:

```shell
sudo apt update
sudo apt upgrade

sudo apt install libsdl2-2.0-0 libsdl2-image-2.0-0 libsdl2-ttf-2.0-0
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

**Tip! If you're having problems installing the SDL2 dev libraries, you may need to change your apt mirror to "Main
Server"**

### Arch:
```shell
sudo pacman -Sy go gcc cmake make sdl2 sdl2_image sdl2_ttf
```


### Mac OS:

#### You are going to need to have [Homebrew](https://brew.sh/) installed

- Install dependencies

```shell
brew install sdl2 sdl2_image sdl2_ttf go
```

## :hammer_and_wrench: Support

Please [open an issue](https://github.com/tedboudros/aurora/issues/new) for support.

## :memo: Contributing

Please contribute using [Github Flow](https://guides.github.com/introduction/flow/). Create a branch, add commits,
and [open a pull request](https://github.com/tedboudros/aurora/compare/).

## :scroll: License

[GNU GPLv3](LICENSE) © [tedboudros](https://github.com/tedboudros)
