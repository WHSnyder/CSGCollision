#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include "utils/ShaderUtils.h"
#include "rendering/Texture.h"

#include <glm.hpp>
#include <ext.hpp>



class Shader {

	private:

		GLuint data_texture, image_texture;
		GLuint proj_loc, view_loc, model_loc, col_loc;

	public:

		GLuint progID;

		Shader(string shaderpath);

		void setModel(float *model);
		void setView(float *view);
		void setProj(float *proj);

		void setFloat(string name, float f);
		void setVec3(string name, glm::vec3 v);
		void setColor(glm::vec3 v);

		int setDataTexture(Texture *tex);
		int setImageTexture(Texture *tex);

		void printUniforms();

		~Shader(){
			glDeleteProgram(progID);
		}
};