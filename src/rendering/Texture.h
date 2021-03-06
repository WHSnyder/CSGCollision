#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>


/*
Basic texture class
*/

class Texture {

	private:

		void *data = nullptr;
		GLuint texID;
		int rows,cols;
		std::string path;

	public:

		Texture(int w, int h, int d);
		Texture(int width, int height);
		Texture(float *data, int width, int height, int color);
		Texture(std::string filepath, int cubemap, std::string extension = std::string(".jpg"));

		inline GLuint getID(){
    		return texID;
		}

		inline int getDim(){
		    return cols;
		}

		~Texture();
};