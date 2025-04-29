#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out float dist;

void main()
{
	color = vertex_color;
	dist = length((view * model * vec4(vertex_position, 1.0)).xyz);

	gl_Position = projection * view * model * vec4(vertex_position, 1.0);
	gl_PointSize = 6.0;
}