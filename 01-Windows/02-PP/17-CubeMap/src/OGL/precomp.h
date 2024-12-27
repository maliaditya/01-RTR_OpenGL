#pragma once

// Standard                                                    
#include <vector>                                                    
#include <string>                                                    
#include <iostream>                                                    
#include <unordered_map>                                                    
#include <cmath>    
#include <filesystem> // Resource


// Assimp                                                    
#include <assimp/Importer.hpp>                                                    
#include <assimp/scene.h>                                                    
#include <assimp/postprocess.h>                                                    

// Window                                                    
#include <windows.h>                                                    
#include <window/Window.h>   


// GLEW                                                    
#include <GL/glew.h>                                                    
#include <GL/gl.h>                                                    

// GLM                                                    
#include <glm/glm.hpp>                                                    
#include <glm/gtc/matrix_transform.hpp>                                                    
#include <glm/gtc/type_ptr.hpp>                                                    

// Utils                                                    
#include  <utils/Logger.h>                                                    
#include  <utils/Time.h>                                                    

// OGL Utils                                                    
#include <OGL/buffer.h>                                                    
#include <OGL/shader.h>                                                    
#include <OGL/texture.h>

// shaders                                                    
#include <OGL/shaders/shaders.h>                                                    
