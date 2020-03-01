#ifndef TEX_
#define TEX_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>


/*
Basic texture class
*/


class Texture {

	public:

		void *data = nullptr;
		GLuint texID;
		int rows,cols;

		Texture(float *data, int width, int height, int color);
		Texture(std::string filepath, int cubemap);

		~Texture(){
			delete data;
			glDeleteTextures(1,&texID);
		}
};
#endif