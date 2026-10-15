# BlockFall

## Disclaimer
This project was made purely to learn new stuff and to keep active while not to think on a game concept.
The similarities to a certain game are real, again this is PURELY to learn and not sell!

## Future Improvments
 - Add sound
 - Build for Mac
 - Build for Mobile, Android and iOS(if possible)
 - Add more game physics values
 - Add menus
 

## Build

### Requirements
- CMake 3.25 or newer
- Ninja
- A Windows C++ toolchain: MSVC with the Windows SDK, or MSYS2 UCRT64 with MinGW-w64 GCC or Clang

### Build info
I used CMake for the first time with (a big) help of ChatGPT to configure, if you have suggestions to improve feel free.
The SDL libraries are not built by this project. CMake downloads the prebuilt packages matching the compiler ABI when they are absent: VC packages for MSVC-compatible toolchains and MinGW packages for GCC or MinGW-targeting Clang. They are extracted under `BlockFall/libs/VC` or `BlockFall/libs/mingw`. The project also downloads nlohmann/json when it is not already present.

The SDL package versions are SDL 3.4.14, SDL_image 3.4.4, SDL_mixer 3.2.4, and SDL_ttf 3.2.2. Custom package roots can be passed with `SDL3_ROOT`, `SDL3_image_ROOT`, `SDL3_mixer_ROOT`, and `SDL3_ttf_ROOT`.

### Build with MSVC
From the `BlockFall` project directory, configure and build with the `windows-ninja` preset:

```powershell
cmake --preset windows-ninja
cmake --build --preset debug
```

### Build with GCC or Clang (without MSVC)
Install CMake, Ninja, and MSYS2 if they are not already installed:

```powershell
winget install --id Kitware.CMake --exact
winget install --id Ninja-build.Ninja --exact
winget install --id MSYS2.MSYS2 --exact
```

Open **MSYS2 UCRT64** from the Start menu and update MSYS2:

```sh
pacman -Syu
```

If the update asks you to close the terminal, close all MSYS2 windows, reopen **MSYS2 UCRT64**, and run the update again. Install Clang and the MinGW binutils (including the resource compiler used by `resources/app.rc`):

```sh
pacman -S --needed mingw-w64-ucrt-x86_64-clang mingw-w64-ucrt-x86_64-binutils
```

For GCC instead, install `mingw-w64-ucrt-x86_64-gcc` in place of the Clang package. Add `C:\msys64\ucrt64\bin` to the Windows user `Path`, then restart VS Code. Use `clang++`, not `clang-cl`; the UCRT64 compiler targets the MinGW ABI.

From the `BlockFall` project directory, configure and build with Clang:

```powershell
cmake -S . -B ../out/build/windows-clang -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/clang++.exe
cmake --build ../out/build/windows-clang --config Debug
```

For GCC, use a separate build directory and the GCC compiler:

```powershell
cmake -S . -B ../out/build/windows-gcc -G "Ninja Multi-Config" -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
cmake --build ../out/build/windows-gcc --config Debug
```

Use a separate build directory for each compiler because CMake caches the selected compiler. In VS Code, choose the matching kit with **CMake: Select a Kit**; after changing compilers, run **CMake: Delete Cache and Reconfigure**. For a Release build, replace `Debug` with `Release`.

### GIF of the Game

![ShareX_2UnIYYXwuE](https://github.com/user-attachments/assets/d4d1e6f9-fa29-44bf-b37c-03b79ba11b79)

### Controls
 - A/D - Move Left/Right
 - S - Soft Drop
 - Space - Rotate Clockwise
 - N - Rotate Counter Clockwise
 - P - Pause
 - G - While in Pause, Toggle Grid Drawing
 - R - On GameOver restarts the game
 - Esc - Exit Game

### Debug Controls
 - 1-6 - Select next Piece
 - Q - Freeze Fall
 - C - Clear field

### Acknowledgements

Want to thank everyone for the assets that I'm using, again this is just for fun and learn, not to get any money.

  - gb-tetris.ttf from Creepercraftyt on https://fontstruct.com/fontstructions/show/2476994/gb-tetris-1
  - BGM.mp3 - A-Type from GB™ Tetris™
  - minos00.png - https://www.spriters-resource.com/nintendo_switch/tetris99/asset/172342/
  - Game Boy _ GBC - Tetris DX - Miscellaneous - Tetriminos.png - https://www.spriters-resource.com/game_boy_gbc/tetrisdx/asset/51442/
  - All Default folder blocks by Vupni - https://archive.org/details/tetris-sprites
  - Game Boy _ GBC - Tetris - Miscellaneous - Block Tiles.png - https://www.spriters-resource.com/game_boy_gbc/tetris/asset/62615/


