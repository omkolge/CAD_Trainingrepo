#version 330 core
layout (location = 0) in vec3 pos;
uniform vec2 posOffset;
out vec3 color;
void main()
{
	gl_Position = vec4(pos.x + posOffset.x ,pos.y + posOffset.y ,pos.z ,1.0);
};