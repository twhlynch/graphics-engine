#version 330 core

layout(location = 0) in vec3 point_position;
layout(location = 1) in float point_size;
layout(location = 2) in vec3 point_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out float dist;

void main()
{
	dist = length((view * model * vec4(point_position, 1.0)).xyz);
	color = point_color;
	gl_Position = projection * view * model * vec4(point_position, 1.0);
	gl_PointSize = point_size;
}
