#version 300 es

in vec3 Position;
out vec3 vPosition;

void main()
{
    vPosition = Position;
    gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);
    gl_PointSize = 10.0;
}