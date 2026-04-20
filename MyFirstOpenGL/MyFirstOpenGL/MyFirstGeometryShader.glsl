#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out; //cuans vertex pot genera


uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {

	for(int i = 0; i < gl_in.length(); i++){
		gl_Position = projectionMatrix * viewMatrix *  gl_in[i].gl_Position;
		EmitVertex();		
	}	

	EndPrimitive();
	
	for(int i = gl_in.length() - 1; i >= 0; i--){
		gl_Position = projectionMatrix * viewMatrix *  (gl_in[i].gl_Position + vec4(1.0,0.0,1.0,0.0));
		EmitVertex();

	}



	EndPrimitive();
}