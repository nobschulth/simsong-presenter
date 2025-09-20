# Simsong Presenter
This is a simple program to present songs from [Songselect](https://songselect.ccli.com/). Just copy a song to your clipboard and paste it using the button. You will automatically get a random design.

## Website
You can find the newest version as github page here: https://nobschulth.github.io/simsong-presenter/

## Building
```bash
git clone https://github.com/nobschulth/simsong-presenter
cd simsong-presenter
git submodule init
git submodule update
lib/SDL_ttf/external/download.sh
mkdir build && cd build
cmake .. -GNinja
ninja
./simsong-presenter
```
To build it as webpage make sure you have the emscripten sdk installed and use emcmake and emmake to build.
