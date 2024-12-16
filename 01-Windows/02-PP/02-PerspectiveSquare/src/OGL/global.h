#pragma once

enum {
ATTRIBUTE_POSITION = 0,
ATTRIBUTE_COLOR,
ATTRIBUTE_NORMAL,
ATTRIBUTE_TEXCOORD,
};

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

// GLM Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/Logger.h"


