#version 420

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texlCoord;

out vec2 fragCoord;

void main()
{
    fragCoord = texlCoord;
    gl_Position = vec4(position,1.0);
}
