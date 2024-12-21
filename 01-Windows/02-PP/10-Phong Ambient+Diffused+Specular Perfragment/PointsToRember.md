## Textures

- Step 1: Get Current Directory Path

```cpp
std::filesystem::path currentPath = std::filesystem::current_path();
```

- step 2: Load texture and enable texture.

```cpp
#include <OGL/textures.h>

GLuint alpha =  loadTexture(std::string(RESOURCE_DIR) + "/textures/floor/alpha.jpg");

// Enabling the texture
glEnable(GL_TEXTURE_2D);
```

## Camera

- camera.positon : tells how much `distance` form the `target position`
- target.position : can be any any where in the `3d space` any Object `positon` which you want to `target`.
- Up: is always `Y` for normal on ground scence, if you are in a plane and you are looking down up will change to `Z`

```z
camera.position = glm::vec3(0.0f,0.0f,10.0f); // positon away form the target
camera.target = walls.position;
camera.updateTransform();
```

## Display

    - step 1. Iterrate over all the meshes
    - step 2. check the scene type and call repective display fucntion.

```cpp
void display()
{
    // code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i=0; i<scene.meshes.size() ; i++)
    {
        if(scene.meshes[i].material.type == BASIC_MATERIAL)
        {
            OGL::displayBasic(camera, scene.meshes[i]);
        }
    }
}
```

## Depth related changes

- step 1: enable depth in initialize
- step 2: add to glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

```cpp
// Depth & Clear Color
glClearDepth(1.0f);
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);

void display()
{
    // code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ...
}



glEnable(GL_PROGRAM_POINT_SIZE);
```

## Blend (transparency)

- Add to initialize

```cpp
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
```

## Points

- Enable Points

```cpp
glEnable(GL_PROGRAM_POINT_SIZE);
```

## Smooth Shading

```cpp
// Optional
glShadeModel(GL_SMOOTH);
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
```
