#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 2) in vec2 texture_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out float dist;

void main()
{
	dist = length((view * model * vec4(vertex_position, 1.0)).xyz);
	texCoord = texture_coord;

	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
}
