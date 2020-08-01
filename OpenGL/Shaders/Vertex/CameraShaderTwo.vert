#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 Tex;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform vec3 position;

void main()
{
	gl_Position = projection * view * model * vec4((aPos + position), 1.0f);
	Tex = aTex;
}