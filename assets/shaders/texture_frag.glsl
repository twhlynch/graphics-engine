#version 330 core

in vec2 texCoord;
in float dist;

out vec4 FragColor;

uniform sampler2D tex;

void main()
{
	float fogStart = 10.0;
	float fogEnd = 30.0;

	float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);

	vec3 fogColor = vec3(0.0);

	vec4 color = texture(tex, texCoord);
	if (color.a == 0.0)
	{
		discard;
	}
	FragColor = vec4(mix(fogColor, color.rgb, fogFactor), color.a / 2.0);
}
