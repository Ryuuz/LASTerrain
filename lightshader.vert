#version 330

layout(location = 0) in vec3 posAttr;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out vec3 norm;
out vec3 fragPos;

uniform mat4 matrix;
uniform mat4 mvMatrix;
uniform mat3 nMatrix;

void main()
{
   norm = nMatrix * normal;
   gl_Position = matrix * vec4(posAttr, 1.0);
   fragPos = vec3(mvMatrix * vec4(posAttr, 1.0));
}
