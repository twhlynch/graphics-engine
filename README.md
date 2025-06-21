# Graphics Engine

![Demo scene](thumb.png)

Almost a simple graphics engine with OpenGL

- [GLAD](https://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D4.6)
- [GLFW 3](https://github.com/glfw/glfw)
- [stb_image](https://github.com/nothings/stb)

### Setup

- Ensure you have installed `cmake`, `opengl`, a generator such as `make`, or `ninja`, and ran `git lfs install`
- Run `git submodule update --init --recursive`
- Run `git lfs pull`
- Run `./build.sh` to build
- Run `./build/default/Renderer` (for Makefiles. Location differs by generator)

#### build config
`./build.sh` to build the debug version

`./build.sh Release` to build the release version (does nothing for some generators)

`./build.sh "CMake Generator"` to build to a specific generator

`./build.sh "CMake Generator" Release` to build to a specific generator in release

E.g.: `./build.sh "Xcode" Release`

### Controls

- `WASD`: move
- `E & Q`: up & down
- `Shift`: sprint
- `ESC`: toggle mouse lock
