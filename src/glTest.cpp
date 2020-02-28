#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string>
#include <chrono>

#include <iostream>
#include <vector>

#include <mesh/Mesh.h>
#include <phys/Physics.h>
#include <gtx/transform.hpp>
#include <perlin/PerlinNoise.hpp>
#include "rendering/Shader.h"
#include "utils/controls.hpp"

//#include "Object.h"

using namespace std;

void coutMat(float *mat){

	cout << "{" << mat[0] << ", " << mat[1] << ", " << mat[2] << ", " << mat[3] << "}" << endl;
	cout << " " << mat[4] << ", " << mat[5] << ", " << mat[6] << ", " << mat[7] << "," << endl;
	cout << " " << mat[8] << ", " << mat[9] << ", " << mat[10] << ", " << mat[11] << "," << endl;
	cout << " " << mat[12] << ", " << mat[13] << ", " << mat[14] << ", " << mat[15] << "}" << endl;
}

float *generate_terrain(int dim, double freq, float height_mult, int32_t *physx_samples){

	const siv::PerlinNoise perlin(16479203914722);
	
	float *result = (float *) calloc(dim * dim, sizeof(float));
	
	double mult = freq/dim;

	for (int i = 0; i < dim; i++){
		for (int j = 0; j < dim; j++){

			float per = (float) perlin.accumulatedOctaveNoise2D_0_1(j * mult, i * mult, 8);

			//if (j % 16 <= 2) per = 1.0;
			if (i == 0 || i == dim - 1) per = 0.0;
			if (j == 0 || j == dim - 1) per = 0.0;
			
			result[(j) * dim + i] = per;

			int32_t cur = (int32_t) height_mult * per;
			physx_samples[i * dim + j] = cur << 16;
		}
	}

	return result;
}


Mesh gen_plane(){

    vec3 verts[] = {vec3(-1,1,0), vec3(1,1,0), vec3(-1,-1,0), vec3(1,-1,0)};
    vec3 norms[] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1)};
    GLuint inds[] = {0,1,2,1,3,2};

    return Mesh(vector<vec3>(verts,verts+4), vector<vec3>(norms,norms+4), vector<GLuint>(inds,inds+6));
}


static void error_callback(int error, const char* description){
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* window;


int main(int argc, char **argv){
	
	Simu mainSimu;

	if(!glfwInit()){
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(600, 600, "Test", NULL, NULL);
	if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  


    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwSetKeyCallback(window, key_callback);


	string path("/Users/will/projects/cpprtx/assets/meshes/cube.obj");
	Mesh cube = Mesh(path);

	string path1("/Users/will/projects/cpprtx/assets/meshes/ball.obj");
	Mesh sphere = Mesh(path1);

	string path2("/Users/will/projects/cpprtx/assets/meshes/terrain_plane.obj");
	Mesh terrain_plane = Mesh(path2);

	Mesh plane = gen_plane();


	int dim = 64;
	double freq = 3.0;

	int32_t *px_samples = (int32_t *) calloc(dim * dim, sizeof(int32_t));

	vec3 terrain_mult = 3.0f * vec3(10.0,3.0,10.0);

    float *img_data = generate_terrain(dim, freq, terrain_mult.y, px_samples);
    Texture noise_tex = Texture(img_data, dim, dim, 0);


	Shader plane_shader = Shader("src/rendering/shaders/plane");
	plane_shader.setDataTexture(&noise_tex);
	
	Shader terrain_shader = Shader("src/rendering/shaders/noise_test");
	terrain_shader.setDataTexture(&noise_tex);
	terrain_shader.setVec3(string("mult"), terrain_mult);

	Shader basic_shader = Shader("src/rendering/shaders/basic");
	glCheckError();


    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 


    auto t_start = std::chrono::high_resolution_clock::now();
	auto t_now = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

	mat4 playerViewMat = lookAt(vec3(18,18,18),vec3(0,0,-10),vec3(0,1.0,0));
	mat4 proj = infinitePerspective(glm::radians(45.0f), 1.0f, 0.01f);
	mat4 rot = mat4(1.0),testmat;
	mat4 trans = translate(vec3(0,9,0));

	mat t1 = translate(vec3(-30.0,0,-30.0)), t2 = translate(vec3(30.0,0,-30.0)), t3 = translate(vec3(-30.0,0,30.0)), t4 = translate(vec3(30.0,0,30.0));
	float *t1p = value_ptr(t1), *t2p = value_ptr(t2), *t3p = value_ptr(t3), *t4p = value_ptr(t4);
	float sphereMat[16] = {}, boxMat[16] = {}; 
	float *viewptr = value_ptr(playerViewMat), *projptr = value_ptr(proj);
	
	mainSimu.addTerrain(px_samples, dim, terrain_mult);
	mainSimu.addSphere(vec3(6,15,6), 1.0, 1);
	mainSimu.addCube(vec3(-9,13,-9), 1.0, 2);

	plane_shader.setProj(projptr);
	basic_shader.setProj(projptr);
	terrain_shader.setProj(projptr);


	do {

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCheckError();

		t_now = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		t_start = t_now;

		mainSimu.stepSimu(time);
		mainSimu.getModelMats(sphereMat, boxMat);

		rot = rotate(rot, time * glm::radians(20.0f), vec3(0.0f,1.0f,0.0f));
		testmat = trans * rot;

		terrain_shader.setModel(value_ptr(testmat));
		terrain_shader.setView(viewptr);
		terrain_plane.draw(terrain_shader.progID);

		plane_shader.setModel(value_ptr(testmat));
		plane_shader.setView(viewptr);
		plane.draw(plane_shader.progID);


		basic_shader.setView(viewptr);
		basic_shader.setColor(vec3(1.0,1.0,1.0));

		basic_shader.setModel(sphereMat);
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(boxMat);
		cube.draw(basic_shader.progID);


		basic_shader.setModel(t1p);
		basic_shader.setColor(2.0f*vec3(1.0,0.0,0.0));
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(t2p);
		basic_shader.setColor(2.0f*vec3(0.0,1.0,0.0));
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(t3p);
		basic_shader.setColor(2.0f*vec3(0.0,0.0,1.0));
		sphere.draw(basic_shader.progID);

		basic_shader.setModel(t4p);
		basic_shader.setColor(2.0f*vec3(1.0,1.0,0.0));
		sphere.draw(basic_shader.progID);


		playerViewMat = computeMatricesFromInputs();
		viewptr = value_ptr(playerViewMat);		

		glfwSwapBuffers(window);
		glfwPollEvents();
	} 

	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	glfwTerminate();

	delete px_samples;


	return 0;
}