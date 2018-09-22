#version 330

layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 colAttr;
layout(location = 2) in vec2 uv;

out vec4 color;
uniform mat4 matrix;

void main()
{
   color = vec4(colAttr, 1.0);
   gl_Position = matrix * vec4(posAttr, 1.0);
}
