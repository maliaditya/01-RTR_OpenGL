#pragma once;
#include <OGL/global.h>

int frameCount = 0;
float fps = 0.0f;

void initializeTimer() 
{
    QueryPerfromanceFrequency(&frequency);
    QueryPerfromanceCounter(&startTime);
}

void calculateFPS()
{
    QueryPerfromanceCounter(&currentTime);

}