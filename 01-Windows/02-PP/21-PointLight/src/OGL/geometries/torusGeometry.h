#pragma once
#include <OGL/global.h>

namespace OGL{
    extern Geometry torusGeometry( float outerRadius = 1.0f,float innerRadius = 0.5f,int numMajor = 36,int numMinor=18);
}