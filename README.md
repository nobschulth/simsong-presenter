# WARNING
> This is a old version and should not be used anymore. Use https://nobschulth.github.io/simsong-presenter-2/ instead

# Simsong Presenter
This is a simple program to present songs from [Songselect](https://songselect.ccli.com/). Just copy a song to your clipboard and paste it using the button. You will automatically get a random design.

## Website
You can find the legacy version as github page here: https://nobschulth.github.io/simsong-presenter/

## Songbook
You can import multiple songs at once and select them from a list. Create a songbook in the following format:
```plaintext
<SONG1>
&&&
<SONG2>
&&&
<SONGX>
```
Replace the `<SONG>` placeholders with your songtext. Then just copy the whole text and paste it using the import songbook button.

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
