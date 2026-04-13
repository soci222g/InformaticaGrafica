#version 440 core

layout (location = 0) in vec3 position;

uniform vec2 offset;

uniform mat4 transform;

void main(){
	
	gl_Position = transform * vec4 (position, 1.0);

}