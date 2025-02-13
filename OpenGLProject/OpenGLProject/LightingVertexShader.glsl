#version 450 core

layout(location = 0) in vec3 vertexPosition; // Each Vertex's Position
layout(location = 1) in vec2 vertexUV; // Each Vertex has texture's Coordinates
layout(location = 2) in vec3 vertexNormal; // Each Vertex has their NormalVector

out vec2 textureCoord; // put vertexUV on FragmentShader
out vec3 normalVector; // put NormalVector on FramentShader
out vec3 o_VertexPosition; // world?????? Vertex ???

uniform mat4 model; // Object's local system -> world system
uniform mat4 view;
uniform mat4 projection; // Project world system to Camera

void main()
{
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0f); // ?????????? ??? ???? Vertex Position?? ???????
	textureCoord = vertexUV; // Texture?? ????? FragmentShader?? ?????
	normalVector = mat3(transpose(inverse(model))) * vertexNormal; // world?????? Normal Vector?? ?????? FragmentShader?? ?????
	o_VertexPosition = vec3(model * vec4(vertexPosition, 1.0)); // world?????? Vertex ????? ?????? FragmentShader?? ?????
}