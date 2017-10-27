#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertexColor;

uniform mat4 P;
uniform mat4 MV;

out vec3 fragColor;
out vec3 vertPosition;

void main()
{
	fragColor = vertexColor;
	gl_Position = P * MV * vertPos;
}
