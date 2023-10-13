#version 300 es

in vec3 Position;
out vec3 vPosition;

uniform float time;

void main()
{
    vPosition = Position + time*vec3(0.01f, 0.0f, 0.0f);
    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
    // gl_PointSize = 10.0;
}