#pragma once 

#include <cstdlib>  // For EXIT_SUCCESS,
#include <OGL/precomp.h>
#include <OGL/global.h>
#include <OGL/geomat.h>
#include <OGL/objects.h>
#include <filesystem> // Resource


extern Scene scene;
extern Camera camera;
extern BOOL GUI;
extern GLuint plfs;
extern std::unique_ptr<ImguiManager> imguiManager;
// Function Declaration
extern int initialize(void);
extern void display(void);
extern void update(void);
extern void uninitialize(void);

extern struct Sizes{
    int width = 800;
    int height = 600;
    int dockedLeft = 0;
    int dockedBottom = 0;
};

extern Sizes sizes;

/* OGL.cpp Template

#include "OGL.h"

Scene scene;
Camera camera;

// Function Declaration
int initialize(void)
{
    // Code


    

    // Depth
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    
    // Texture
    glEnable(GL_TEXTURE_2D);

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Smooth
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // Points
    glEnable(GL_PROGRAM_POINT_SIZE);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    return(0);
}

void display(void)
{
    // Code
    
}

void update(void)
{
    // Code
    
}

void uninitialize(void)
{
    // Code
    
}

*/





