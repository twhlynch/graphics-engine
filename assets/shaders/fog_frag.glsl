#version 330 core

in vec3 color;
in float dist;
out vec4 FragColor;

void main()
{
	float fogStart = 10.0;
	float fogEnd = 30.0;

	float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);

	vec3 fogColor = vec3(0.0, 0.0, 0.0);
	FragColor = vec4(mix(fogColor, color, fogFactor), 1.0);
}