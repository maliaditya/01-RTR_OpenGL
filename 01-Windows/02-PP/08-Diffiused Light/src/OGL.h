#pragma once 

#include <cstdlib>  // For EXIT_SUCCESS,
#include <OGL/precomp.h>
#include <OGL/global.h>
#include <OGL/geomat.h>
#include <OGL/objects.h>
#include <filesystem> // Resource


extern Scene scene;
extern Camera camera;

// Function Declaration
extern int initialize(void);
extern void display(void);
extern void update(void);
extern void uninitialize(void);


/* OGL.cpp Template

#include "OGL.h"

Scene scene;
Camera camera;

// Function Declaration
int initialize(void)
{
    // Code
    
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





