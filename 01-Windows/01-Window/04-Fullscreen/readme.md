# Window FullScreen

## Output (full white screen)

![alt text](output.png)

## Description

- This is just a empty window using Win32 API with below feature
  - Open in Center of the screen.
  - Icons on window and tasbar.
  - Fullscreen toggle on pressing 'f'/'F'.
  - A Logging mechanism with File I/O.

## Steps to build

    1. Download the code
    2. Open CMD in this folder.
    3. mkdir build && cd build
    4. cmake ..
    5. cmake --build . --config Debug
    6. Debug\OGL.exe
        OR
    7. double click on the OGL.exe in build/debug/OGL.exe
