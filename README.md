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
 - Visual Studio 2022 (using C++ 23, but maybe 20 is enough)

### Build info
I used CMake for the first time with (a big) help of ChatGPT to configure, if you have suggestions to improve feel free.
Everything except the SDL3_mixer that it's not officially released, and needs to be build from source or find somewhere where it's build and put manually on the correct folder, should download automatically and be put on the correct folder.

### Build steps
 - Clone the repo
 - Create a 'build' folder on the 'BlockFall' folder (alonside source, asset etc folders)
 - On the Powershell run these commands
  - cd /path/to/the/Project/BlockFall/build
  - CMake .. -G "Visual Studio 17 2022" -A x64
- Wait for the project to generate
- Manually download/build SDL3_mixer and put the files on the correct folders:
 - SDL3_mixer.dll and SDL3_mixer.lib - ./ProjectFolder/BlockFall/libs/SDL3/lib/x64
 - SDL3_mixer.h - ./ProjectFolder/BlockFall/libs/SDL3/include/SDL3

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


