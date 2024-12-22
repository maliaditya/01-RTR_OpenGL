# Phong Ambient + Diffuse + Specular Light - PerFragment

## Build Steps

1. Download the code.
2. Open CMD in the project folder.
3. Run the following commands:
   - `mkdir build && cd build`
   - `cmake ..`
   - `cmake --build . --config Debug`
4. To run the program:
   - `Debug\OGL.exe` (from the build folder)
     OR
   - Double-click `OGL.exe` in `build/Debug/`.

## Output

![alt text](output.png)

## Technical Details

### Window

- **Created using the Win32 API**.
- **Centered on the screen**.
- **Icon**: Added to both window and taskbar.
- **Fullscreen toggle**: Press 'f'/'F' to toggle fullscreen mode.
- **Focus handling**: Kills focus when the window is inactive.
- **Logging**: A file I/O-based logging system is implemented, supporting INFO, DEBUG, WARN, and ERROR levels.

### OpenGL

- **Procedural Shapes**:

  - Cone
  - Cube
  - Sphere
  - Plane
  - Torus

- **Structural Changes**:

  - **Geometry**: Holds vertex data.
  - **Material**: Stores shaders.
  - **Mesh**: Stores all object data.
  - **Scene**: Manages objects and meshes.
  - **Camera**: Added Camera struct for easier scene navigation.
  - **Logger**: Now supports different log levels (INFO, DEBUG, WARN, ERROR).

- **Multitexturing**:

  - Alpha blending
  - Displacement
  - Color maps
  - Normals

- **Camera**:

  - **Orbit-style camera**: Rotate the scene using mouse drag (L button pressed); stops rotating when the L button is released.
  - **Targeting**: Once set, the camera targets an object and looks at it.

- **Lighting**:
  - `Diffuse` light added to the scene.
  - Added PerVertex Phong `Ambient` + `Diffuse` + `SpecularLight`.
  - Added PerFragment Phong `Ambient` + `Diffuse` + `SpecularLight`.

### Build Tool (CMake)

- Added precompiled header for faster compilation.

### Third-Party Libraries Used

- **GLM**: For math operations.
- **stb_image**: For texture loading.

## Dependencies

To build and run this project, you need the following dependencies:

- **Windows OS**
- **MSVC 17 (Visual Studio 2019/2022)**: Microsoft Visual Studio 2019 or 2022 with the MSVC 17 compiler.
- **CMake**: For building the project.
- **Ninja**: A build system used with CMake to speed up the build process.
- **GLM**: (automatically fetched from GitHub).
- **stb_image**: (automatically fetched from GitHub).
- **GLEW**: OpenGL Extension Wrangler Library (automatically fetched from GitHub).
