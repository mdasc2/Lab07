#version 330 core 

uniform vec3 uColor;

//in vec3 fragNor;
in vec3 vColor;
in vec3 test;
out vec4 color;

void main()
{

	//vec3 normal = normalize(fragNor);
	// Map normal in the range [-1, 1] to color in range [0, 1];
	//vec3 Ncolor = 0.5*normal + 0.5;

	color = vec4(vColor,0.0);

}
