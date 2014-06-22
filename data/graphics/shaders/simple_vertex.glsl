#version 130
in ivec4 position;
out vec2 Texcoord;
uniform vec2 screen;
void main()
{
    Texcoord = vec2(position.z, position.w);
    gl_Position = vec4(position.x / screen.x - 1, 1 - position.y / screen.y, 0.0, 1.0);
}
