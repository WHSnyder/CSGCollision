#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 p, v, m;

void main(){

	vec4 t = p * v * m * vec4(pos,1.0);
	t.w = 1.0;
	t.z = .5;
	gl_Position = t;
}