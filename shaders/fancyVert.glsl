#version 300 es

in vec3 Position;
out vec3 vPosition;

void main()
{
    vPosition = Position + vec3(0.00f, 0.0f, 0.0f);
    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
    // gl_PointSize = 10.0;
}