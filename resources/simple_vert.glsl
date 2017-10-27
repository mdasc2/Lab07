#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;

uniform vec3 LightVector;
uniform mat4 P;
uniform mat4 MV;
uniform vec3 uColor;

out vec3 fragNor;
out vec3 vColor;
out vec3 test;


void main()
{

	vec3 modelViewVertex = vec3(MV * vertPos);


	vec3 modelViewNormal = vec3(MV * vec4(vertNor, 0.0));

	float distance =  length(LightVector - modelViewVertex);

	vec3 lightVector = normalize(LightVector - modelViewVertex);

	float diffuse  = max(dot(modelViewNormal,lightVector), 0.0);

	diffuse  = diffuse * 10.0;

	vColor = uColor  * diffuse;

	gl_Position = P * MV * vertPos;
	//fragNor = (MV * vec4(vertNor, 0.0)).xyz;
}
