#pragma once 

#include <OGL/precomp.h>
#include <OGL/global.h>
#include <OGL/geomat.h>
#include <OGL/objects.h>
#include "window/resource.h"

#define WINWIDTH 800
#define WINHEIGHT 600

extern Scene scene;
extern Camera camera;


// Function Declaration

extern  int initialize(void);

extern  void resize(int width, int height);

extern  void display(void);

extern  void update(void);

extern  void uninitialize(void);