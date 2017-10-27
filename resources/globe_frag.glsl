#version 330 core 

uniform vec3 uColor;
uniform vec3 backGroundColor;
uniform vec2 uWindowSize;

in vec3 vertPosition;
in vec3 fragColor;
out vec4 color;




void main()
{	
	vec2 center = uWindowSize / 2.0;

	color = vec4(uColor, 1.0);

	if(gl_FragCoord.z < 0.017 )
	{
		discard;
	}
	else if(gl_FragCoord.z < 0.055)
	{
		color = vec4(backGroundColor, 1.0);
	}
	else
	{
		float ratio = gl_FragCoord.z/0.18;
		float Oratio = 1-ratio;
		
		color.r = color.r * ratio + 1.0 * Oratio;
		color.g = color.g * ratio + 1.0 * Oratio;
		color.b = color.b * ratio + 1.0 * Oratio;

	}
}
