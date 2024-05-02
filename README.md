# Description
Basic 3D renderer.

# Requirements
- CMake >= 3.22
- C++17 compliant compiler
- Armadillo library
- SFML library

# How to build

```bash
git clone https://github.com/roghed/perspective-projection.git
cd perspective-projection
cmake -S . -B build
cmake --build build
```

After that the executable file will be located at `./build/perspective-projection`

# How to run

Launch `perspective-projection` executable from the project **root** directory. The executable will look for `data/` and `scene/` directories in the current working directory so make sure you do not launch it from the `build/` directory.

```bash
build/perspective-projection
```

# Controls

- `W`, `A`, `S`, `D` - move camera forward/backwards, left/right
- `Space`, `Left Shift` - move camera up/down
- `E`, `Q` - spin camera left/right
- `Mouse` - pan camera left/right, up/down
- `Mouse wheel` - change camera field of view
- `Enter` - toggle wireframe rendering
- `B` - toggle draw order coloring (cold colors are rendered first, warmer later)