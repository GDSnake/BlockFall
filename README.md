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
 - CMake
 - Visual Studio Code
 - Internet access on the first CMake configure (to download the prebuilt packages)
 - Prebuilt SDL3 development packages (downloaded automatically when absent):
   - SDL 3.4.14
	  - SDL_image 3.4.4
   - SDL_mixer 3.2.4
   - SDL_ttf 3.2.2

### Build info
I used CMake for the first time with (a big) help of ChatGPT to configure, if you have suggestions to improve feel free.
The SDL libraries are not built by this project. On Windows x64, CMake downloads the matching prebuilt VC development packages when they are absent and extracts them under `BlockFall/libs/SDL3`, `BlockFall/libs/SDL3_image`, `BlockFall/libs/SDL3_mixer`, and `BlockFall/libs/SDL3_ttf`. Custom package roots can be passed with `SDL3_ROOT`, `SDL3_image_ROOT`, `SDL3_mixer_ROOT`, and `SDL3_ttf_ROOT`.

### Build steps
 - Clone the repo
   - From the `BlockFall` project directory, run these PowerShell commands:
  - `cd BlockFall`
  - `cmake --preset windows-ninja`
  - `cmake --build --preset windows-ninja-debug`
  - Wait for CMake to download and verify all four exact SDL package versions when needed.

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


