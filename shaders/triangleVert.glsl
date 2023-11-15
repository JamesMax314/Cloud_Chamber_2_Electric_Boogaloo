#version 300 es

layout (location = 0) in vec3 aPos;
out vec3 fragPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    fragPos = vec3(aPos.x, aPos.y, aPos.z);
}
