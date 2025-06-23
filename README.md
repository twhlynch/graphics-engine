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
- Run `./build.sh -r` to build and run

#### build script

```bash
  usage: ./build.sh [options] ["CMake Generator"]
         Build the project with a specified generator (or 'default')
options:
         -R --release     Build Release target
         -d --docs        Build Doxygen documentation
         -r --run         Run after build
         -o --optimise    Adds the -O3 compile option
         -h --help        Show this message
```

`chmod +x build.sh` if needed.

By default `./build.sh` will build the Debug version with your default CMake Generator (usually Unix Makefiles).

You can optionally add `-R` to build for release, `-d` to also generate documentation with Doxygen (must be installed), `-r` to automatically run, `-o` to use -O3 compiler optimisation, or specify a different CMake Generator such as `Xcode`.

The build output will be in `build/default`, replacing default with a generator if specified, and within `Debug` or `Release` if supported by the generator you use.

E.g.: `./build.sh "Xcode" -R` will build with `Xcode` to `build/Xcode/Release`.

E.g.: `./build.sh -R -d` will build with the default generator (usually Makefiles) to `build/default` and generate documentation to `build/default/html`.

> Note: Sometimes generating docs will require `sudo`

### Controls

- `WASD`: move
- `E & Q`: up & down
- `Shift`: sprint
- `ESC`: toggle mouse lock
- `SPACE`: cast ray
