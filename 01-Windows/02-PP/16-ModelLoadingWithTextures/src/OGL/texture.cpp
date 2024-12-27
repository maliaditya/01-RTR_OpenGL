#include "texture.h"
#define STBI_ASSERT(x) if (!(x)) { std::cerr << "STBI_ASSERT failed: " << #x << std::endl; }
#define STB_IMAGE_IMPLEMENTATION  
#include <utils/stb_image.h>
namespace OGL {
	Logger tex("Texture.log");

	   GLuint   loadTexture(const std::string& filename) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture wrapping and filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load texture image
		int width, height, channels;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
		if (data) {
			GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			tex.error("Filename: ",filename);
			tex.error("Failed to load texture: " , stbi_failure_reason());
			glDeleteTextures(1, &textureID);
			textureID = 0;
		}

		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}


	 BOOL loadGLTexture(GLuint* texture, TCHAR imageResourceID[])
	{
		// Variable declarations
		HBITMAP hBitmap;
		BITMAP bmp;
		BOOL bresult = FALSE;

		// code
		hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

		if (hBitmap)
		{
			bresult = TRUE;
			GetObject(hBitmap, sizeof(bmp), &bmp);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glBindTexture(GL_TEXTURE_2D, *texture);

			// Create texture
			//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bmp.bmBits);
			glGenerateMipmap(GL_TEXTURE_2D);

			// unbind
			glBindTexture(GL_TEXTURE_2D, 0);
			DeleteObject(hBitmap);
		}
		return bresult;

	}

	 GLuint loadCubemap(std::vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				tex.error("Cubemap texture failed to load at path: ", faces[i]) ;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}



	 GLuint loadTextureEmbedded(void* pData, int bufferSize) {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Set texture wrapping and filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Flip the image vertically (optional)
		stbi_set_flip_vertically_on_load(true);

		// Load texture image and generate mipmaps
		int width, height, channels;
		unsigned char* data = stbi_load_from_memory((const stbi_uc*)pData, bufferSize, &width, &height, &channels, 0);
		if (data) {
			GLenum format;
			if (channels == 1) {
				format = GL_RED;
			} else if (channels == 3) {
				format = GL_RGB;
			} else if (channels == 4) {
				format = GL_RGBA;
			} else {
				std::cerr << "Unsupported image format." << std::endl;
				stbi_image_free(data);
				return 0;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cerr << "Failed to load texture from memory." << std::endl;
			return 0; // Return 0 to indicate failure
		}

		// Free image data and unbind texture
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}

}