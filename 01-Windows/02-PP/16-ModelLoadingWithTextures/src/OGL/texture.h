#pragma once
#include "global.h"

namespace OGL{
	extern  GLuint  loadTexture(const std::string& filename) ;
	extern  BOOL loadGLTexture(GLuint* texture, TCHAR imageResourceID[]);
	extern  GLuint loadCubemap(const std::vector<std::string>& faces);
	extern  GLuint loadTextureEmbedded(void* pData, int bufferSize) ;
}