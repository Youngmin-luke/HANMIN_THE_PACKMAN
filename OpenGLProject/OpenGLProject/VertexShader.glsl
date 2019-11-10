#version 450 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
out vec2 textureCoord;

uniform mat4 MVP;

void main()
{
	gl_Position =  vec4(vertexPosition, 1.0f);
	textureCoord = vertexUV;
}