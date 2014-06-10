#version 150 core
in vec2 position;
uniform vec2 screen;
void main()
{
    gl_Position = vec4(position.x / screen.x - 1, 1 - position.y / screen.y, 0.0, 1.0);
}
