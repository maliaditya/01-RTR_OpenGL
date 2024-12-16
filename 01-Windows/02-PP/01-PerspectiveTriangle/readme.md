# Perspective triangle

## Output

![alt text](output.png)

## Description

- Window created using Win32 API with below feature
  - Open in Center of the screen.
  - Add Icon on window and taskbar.
  - Fullscreen toggle on pressing 'f'/'F'.
  - A Logging mechanism with File I/O.
  - On Window active setfocus Kill focus.
- OpenGL
  - Perspective triangle created in OpenGL.

## Steps to build

    1. Download the code
    2. Open CMD in this folder.
    3. mkdir build && cd build
    4. cmake ..
    5. cmake --build . --config Debug
    6. Debug\OGL.exe
        OR
    7. double click on the OGL.exe in build/debug/OGL.exe
